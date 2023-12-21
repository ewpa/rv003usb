#ifndef _USB_CONFIG_H
#define _USB_CONFIG_H

//Defines the number of endpoints for this device. (Always add one for EP0). For two EPs, this should be 3.
#define ENDPOINTS 2

#define USB_DM 4
#define USB_DP 2
#define USB_DPU 1
#define USB_PORT C

#define RV003USB_DEBUG_TIMING      0
#define RV003USB_OPTIMIZE_FLASH    0
#define RV003USB_EVENT_DEBUGGING   0
#define RV003USB_HANDLE_IN_REQUEST 1
#define RV003USB_OTHER_CONTROL     0
#define RV003USB_HANDLE_USER_DATA  0
#define RV003USB_HID_FEATURES      0

#ifndef __ASSEMBLER__

#include <tusb_types.h>
#include <tinyusb_hid.h>

#ifdef INSTANCE_DESCRIPTORS

// Taken from http://www.usbmadesimple.co.uk/ums_ms_desc_dev.htm
static const uint8_t device_descriptor[] = {
        18, //Length
        TUSB_DESC_DEVICE,  //Type (Device)
        0x10, 0x01, //Spec
        TUSB_CLASS_HID, //Device Class
        0x0, //Device Subclass
        0x0, //Device Protocol  (000 = use config descriptor)
        0x08, //Max packet size for EP0 (This has to be 8 because of the USB Low-Speed Standard)
        0xfe, 0xca, //ID Vendor
        0x04, 0x40, //ID Product
        0x07, 0x00, //ID Rev
        1, //Manufacturer string
        2, //Product string
        3, //Serial string
        1, //Max number of configurations
};

static const uint8_t mouse_hid_desc[] = {  //From http://eleccelerator.com/tutorial-about-usb-hid-report-descriptors/
        HID_USAGE_PAGE( HID_USAGE_PAGE_DESKTOP ),         // USAGE_PAGE (Generic Desktop)
        HID_USAGE( HID_USAGE_DESKTOP_MOUSE ),             // USAGE (Mouse)
        HID_COLLECTION ( HID_COLLECTION_APPLICATION ),    // COLLECTION (Application)
                HID_USAGE( HID_USAGE_DESKTOP_POINTER ),       //   USAGE (Pointer)
                HID_COLLECTION ( HID_COLLECTION_PHYSICAL ),   //   COLLECTION (Physical)
                        HID_USAGE_PAGE( HID_USAGE_PAGE_BUTTON ),  //     USAGE_PAGE (Button)
                        HID_USAGE_MIN( 1 ),                       //     USAGE_MINIMUM (Button 1)
                        HID_USAGE_MAX( 5 ),                       //     USAGE_MAXIMUM (Button 5)
                        HID_LOGICAL_MIN( 0 ),                     //     LOGICAL_MINIMUM (0)
                        HID_LOGICAL_MAX( 1 ),                     //     LOGICAL_MAXIMUM (1)
                        HID_REPORT_COUNT( 5 ),                    //     REPORT_COUNT (5)
                        HID_REPORT_SIZE( 1 ),                     //     REPORT_SIZE (1)
                        HID_INPUT( 0x02 ),                        //     INPUT (Data,Var,Abs)
                        HID_REPORT_COUNT( 1 ),                    //     REPORT_COUNT (1)
                        HID_REPORT_SIZE( 3 ),                     //     REPORT_SIZE (3)
                        HID_INPUT( 0x01 ),                        //     INPUT (Cnst,Arr,Abs)
                        HID_USAGE_PAGE( HID_USAGE_PAGE_DESKTOP ), //     USAGE_PAGE (Desktop)
                        HID_USAGE( HID_USAGE_DESKTOP_X ),         //     USAGE (X)
                        HID_USAGE( HID_USAGE_DESKTOP_Y ),         //     USAGE (Y)
                        HID_USAGE( HID_USAGE_DESKTOP_WHEEL ),     //     USAGE (Wheel)
                        HID_LOGICAL_MIN( 0x81 ),                  //     LOGICAL_MINIMUM -127
                        HID_LOGICAL_MAX( 0x7f ),                  //     LOGICAL_MAXIMUM (127)
                        HID_REPORT_SIZE( 8 ),                     //     REPORT_SIZE (8)
                        HID_REPORT_COUNT( 3 ),                    //     REPORT_COUNT (3)
                        HID_INPUT( 0x06 ),                        //     INPUT (Data,Var,Rel)
                HID_COLLECTION_END,                           //   END_COLLECTION
        HID_COLLECTION_END,                               // END_COLLECTIONs
};

static const uint8_t config_descriptor[] = {
        // configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
        9,                        // bLength;
        TUSB_DESC_CONFIGURATION,  // bDescriptorType;
        0x22, 0x00,               // wTotalLength
        0x01,                     // bNumInterfaces (Normally 1)
        0x01,                     // bConfigurationValue
        0x00,                     // iConfiguration
        0x80,                     // bmAttributes (was 0xa0)
        0x32,                     // bMaxPower (100mA)

        //Mouse
        9,                        // bLength
        TUSB_DESC_INTERFACE,      // bDescriptorType
        0,                        // bInterfaceNumber  = 1 instead of 0 -- well make it second.
        0,                        // bAlternateSetting
        1,                        // bNumEndpoints
        TUSB_CLASS_HID,           // bInterfaceClass (0x03 = HID)
        HID_SUBCLASS_BOOT,        // bInterfaceSubClass
        HID_PROTOCOL_MOUSE,        // bInterfaceProtocol (1 = Keyboard, 2 = Mouse)
        2,                        // iInterface

        9,                        // bLength
        HID_DESC_TYPE_HID,        // bDescriptorType (HID)
        0x10,0x01,                // bcd 1.10
        0x00,                     //country code
        0x01,                     // Num descriptors
        HID_DESC_TYPE_REPORT,     // DescriptorType[0] (HID)
        sizeof(mouse_hid_desc), 0x00,     // Descriptor length XXX This looks wrong!!!

        7,                        // endpoint descriptor (For endpoint 1)
        TUSB_DESC_ENDPOINT,       // Endpoint Descriptor (Must be 5)
        0x81,                     // Endpoint Address
        0x03,                     // Attributes
        0x08, 0x00,               // Size
        1,                        // Interval (Was 0x0a)
};

#define STR_MANUFACTURER u"Generic"
#define STR_PRODUCT      u"Mouse"
#define STR_SERIAL       u"123456"

struct usb_string_descriptor_struct {
	uint8_t bLength;
	uint8_t bDescriptorType;
	uint16_t wString[];
};
const static struct usb_string_descriptor_struct string0 __attribute__((section(".rodata"))) = {
	4,
	3,
	{0x0409}
};
const static struct usb_string_descriptor_struct string1 __attribute__((section(".rodata")))  = {
	sizeof(STR_MANUFACTURER),
	3,
	STR_MANUFACTURER
};
const static struct usb_string_descriptor_struct string2 __attribute__((section(".rodata")))  = {
	sizeof(STR_PRODUCT),
	3,
	STR_PRODUCT
};
const static struct usb_string_descriptor_struct string3 __attribute__((section(".rodata")))  = {
	sizeof(STR_SERIAL),
	3,
	STR_SERIAL
};

// This table defines which descriptor data is sent for each specific
// request from the host (in wValue and wIndex).
const static struct descriptor_list_struct {
        uint32_t        lIndexValue;
        const uint8_t   *addr;
        uint8_t         length;
} descriptor_list[] = {
        {0x00000100, device_descriptor, sizeof(device_descriptor)},
        {0x00000200, config_descriptor, sizeof(config_descriptor)},
        // interface number // 2200 for hid descriptors.
        {0x00002200, mouse_hid_desc, sizeof(mouse_hid_desc)},
        {0x00002100, config_descriptor + 18, 9 }, // Not sure why, this seems to be useful for Windows + Android.

        {0x00000300, (const uint8_t *)&string0, 4},
        {0x04090301, (const uint8_t *)&string1, sizeof(STR_MANUFACTURER)},
        {0x04090302, (const uint8_t *)&string2, sizeof(STR_PRODUCT)},
        {0x04090303, (const uint8_t *)&string3, sizeof(STR_SERIAL)}
};
#define DESCRIPTOR_LIST_ENTRIES ((sizeof(descriptor_list))/(sizeof(struct descriptor_list_struct)) )

#endif // INSTANCE_DESCRIPTORS

#endif

#endif
