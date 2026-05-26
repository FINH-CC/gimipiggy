//
// Copyright 2026.
//

#ifndef __GIMI_PB_SERVER_H
#define __GIMI_PB_SERVER_H

#define SLEEP_TIME_BETWEEN_FETCH 60000000 // In microseconds. 1,000,000 us= 1 second. 60000000 = 60 seconds.

#define MAX_URL_LENGTH 256
#define ETAG_SIZE 64
#define GIMI_PB_RECEIPT_NULL_ETAG "0000000000000000000000000000000000000000000000000000000000000000"

#define GIMI_PB_RECEIPTS_TOTAL 6

enum gimi_pb_receipt_type {
    GIMI_PB_RECEIPT_TYPE_WELCOME = 0,
    GIMI_PB_RECEIPT_TYPE_DEFAULT,
    GIMI_PB_RECEIPT_TYPE_SAVINGS,
    GIMI_PB_RECEIPT_TYPE_CELEBRATION,
    GIMI_PB_RECEIPT_TYPE_REMINDER,
    GIMI_PB_RECEIPT_TYPE_NOTIFICATION
};

// Uncomment ONLY ONE block of URL's below, to select which server (and/or which user profile that you want THIS device to use).

// pig_001 Real live FINH server.
/*#define GIMI_PB_RECEIPT_WELCOME_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_001/welcome.bin"
#define GIMI_PB_RECEIPT_DEFAULT_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_001/default.bin"
#define GIMI_PB_RECEIPT_SAVINGS_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_001/savings.bin"
#define GIMI_PB_RECEIPT_CELEBRATION_URL  "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_001/celebration.bin"
#define GIMI_PB_RECEIPT_REMINDER_URL     "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_001/reminder.bin"
#define GIMI_PB_RECEIPT_NOTIFICATION_URL "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_001/notification.bin"*/

// pig_002 Real live FINH server.
/*#define GIMI_PB_RECEIPT_WELCOME_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_002/welcome.bin"
#define GIMI_PB_RECEIPT_DEAFULT_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_002/default.bin"
#define GIMI_PB_RECEIPT_SAVINGS_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_002/savings.bin"
#define GIMI_PB_RECEIPT_CELEBRATION_URL  "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_002/celebration.bin"
#define GIMI_PB_RECEIPT_REMINDER_URL     "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_002/reminder.bin"
#define GIMI_PB_RECEIPT_NOTIFICATION_URL "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_002/notification.bin"*/

// pig_003 Real live FINH server.
/*#define GIMI_PB_RECEIPT_WELCOME_URL     "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_003/welcome.bin"
#define GIMI_PB_RECEIPT_DEFAULT_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_003/default.bin"
#define GIMI_PB_RECEIPT_SAVINGS_URL      "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_003/savings.bin"
#define GIMI_PB_RECEIPT_CELEBRATION_URL  "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_003/celebration.bin"
#define GIMI_PB_RECEIPT_REMINDER_URL     "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_003/reminder.bin"
#define GIMI_PB_RECEIPT_NOTIFICATION_URL "https://pudixovgv7mqdvud.public.blob.vercel-storage.com/pig_003/notification.bin"*/

// Dom's Github testbed.
#define GIMI_PB_RECEIPT_WELCOME_URL      "https://raw.githubusercontent.com/dostr/dostr.github.io/master/w384_type_test_384_1047.bin"
#define GIMI_PB_RECEIPT_DEFAULT_URL      "https://raw.githubusercontent.com/dostr/dostr.github.io/master/w384_type_test_384_1047.bin"
#define GIMI_PB_RECEIPT_SAVINGS_URL      "https://raw.githubusercontent.com/dostr/dostr.github.io/master/w384_type_test_384_1047.bin"
#define GIMI_PB_RECEIPT_CELEBRATION_URL  "https://raw.githubusercontent.com/dostr/dostr.github.io/master/w384_type_test_384_1047.bin"
#define GIMI_PB_RECEIPT_REMINDER_URL     "https://raw.githubusercontent.com/dostr/dostr.github.io/master/w384_type_test_384_1047.bin"
#define GIMI_PB_RECEIPT_NOTIFICATION_URL "https://raw.githubusercontent.com/dostr/dostr.github.io/master/w384_type_test_384_1047.bin"

#endif // __GIMI_PB_SERVER_H


