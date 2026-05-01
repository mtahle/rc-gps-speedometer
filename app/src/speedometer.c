/*
 * RC GPS Speedometer — POC firmware for AI-Thinker A7 (GPS+GPRS)
 *
 * Uses the event-based GPS API (same pattern as sdk/demo/gps/):
 *   1. GPS_Open(NULL) — SDK delivers NMEA via API_EVENT_ID_GPS_UART_RECEIVED
 *   2. GPS_Update()   — SDK parses NMEA into GPS_Info_t (minmea-backed structs)
 *   3. gpsInfo->rmc.speed — speed in knots as minmea_float; converted to km/h
 *
 * Speed output via Trace() using integer km/h (avoids %f which Trace unsupports).
 * Entry point: app_Main()  (matches ./build.sh app naming convention)
 */

#include <string.h>
#include <api_os.h>
#include <api_debug.h>
#include <api_event.h>
#include <api_gps.h>
#include <api_hal_uart.h>
#include "gps.h"

/* ------------------------------------------------------------------ */
/*  Task config                                                         */
/* ------------------------------------------------------------------ */
#define MAIN_TASK_STACK  (2048 * 2)
#define MAIN_TASK_PRIO   0
#define GPS_TASK_STACK   (2048 * 2)
#define GPS_TASK_PRIO    1

static HANDLE s_mainHandle = NULL;
static HANDLE s_gpsHandle  = NULL;

/* ------------------------------------------------------------------ */
/*  GPS worker task                                                     */
/*  Waits for a valid fix, then prints speed every 1 s.               */
/* ------------------------------------------------------------------ */
static void gps_task(void *pData)
{
    (void)pData;

    GPS_Info_t *info = Gps_GetInfo();

    GPS_Init();
    GPS_Open(NULL);     /* NULL → SDK uses event system for NMEA delivery */

    Trace(1, "[spd] GPS open, waiting for fix...");

    /* Wait until RMC is populated (latitude non-zero = first valid fix) */
    while (info->rmc.latitude.value == 0)
        OS_Sleep(1000);

    Trace(1, "[spd] GPS fixed");

    while (1) {
        /* minmea_float → integer: value / scale gives the floating value.
         * knots * 1852 / 1000 = km/h  (all integer, no printf %f needed) */
        int32_t speed_x1000 = (int32_t)info->rmc.speed.value * 1852
                              / info->rmc.speed.scale;      /* km/h * 1000 */
        int32_t kmh_int  = speed_x1000 / 1000;
        int32_t kmh_frac = (speed_x1000 % 1000) / 10;      /* two decimals */

        if (info->rmc.valid) {
            Trace(1, "[spd] %d.%02d km/h", (int)kmh_int, (int)kmh_frac);
        } else {
            Trace(1, "[spd] no fix");
        }

        OS_Sleep(1000);
    }
}

/* ------------------------------------------------------------------ */
/*  System event dispatcher                                             */
/* ------------------------------------------------------------------ */
static void event_dispatch(API_Event_t *e)
{
    switch (e->id) {
        case API_EVENT_ID_GPS_UART_RECEIVED:
            /* Feed raw NMEA bytes to the SDK GPS parser */
            GPS_Update(e->pParam1, e->param1);
            break;

        case API_EVENT_ID_NETWORK_REGISTERED_HOME:
        case API_EVENT_ID_NETWORK_REGISTERED_ROAMING:
            Trace(1, "[spd] GPRS registered");
            break;

        default:
            break;
    }
}

/* ------------------------------------------------------------------ */
/*  Main task — event loop + spawns GPS task                           */
/* ------------------------------------------------------------------ */
static void main_task(void *pData)
{
    (void)pData;

    Trace(1, "[spd] RC GPS Speedometer POC starting");

    s_gpsHandle = OS_CreateTask(gps_task,
                                NULL, NULL,
                                GPS_TASK_STACK, GPS_TASK_PRIO,
                                0, 0, "SpeedometerGPS");

    API_Event_t *event = NULL;
    while (1) {
        if (OS_WaitEvent(s_mainHandle, (void **)&event,
                         OS_TIME_OUT_WAIT_FOREVER)) {
            event_dispatch(event);
            OS_Free(event->pParam1);
            OS_Free(event->pParam2);
            OS_Free(event);
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Firmware entry point                                                */
/* ------------------------------------------------------------------ */
void app_Main(void)
{
    s_mainHandle = OS_CreateTask(main_task,
                                 NULL, NULL,
                                 MAIN_TASK_STACK, MAIN_TASK_PRIO,
                                 0, 0, "SpeedometerMain");
    OS_SetUserMainHandle(&s_mainHandle);
}
