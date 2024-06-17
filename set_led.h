#ifndef SET_LED_H
#define SET_LED_H

#define LED_SCROLL_WHEEL 0x01
#define LED_LOGO 0x04

/* Each report has 90 bytes */
#define RAZER_FEATURE_REPORT_LEN 0x5A

struct rgb {
    unsigned char r,g,b; /* 0-255 */
};

/* Report structure */
union transaction_id_union {
    unsigned char id;
    struct transaction_parts {
        unsigned char device : 3;
        unsigned char id : 5;
    } parts;
};
union command_id_union {
    unsigned char id;
    struct command_id_parts {
        unsigned char direction : 1;
        unsigned char id : 7;
    } parts;
};
struct razer_report {
    unsigned char start; /* 0x00 */
    union transaction_id_union transaction_id; /* 0x1f */
    unsigned char reserved[3]; /* 0x000000 */
    unsigned char arguments_num;
    unsigned char command_class;
    union command_id_union command_id;
    unsigned char arguments[80]; /* [0]==0 no store, ==1 store */
    unsigned char checksum;/*xor'ed bytes of report*/
    unsigned char reserved1; /*0x0*/
};
/* common functions called by device attributes store functions */
ssize_t attr_write_test_common(struct device *dev, struct device_attribute *attr, const char *buf, size_t count, unsigned char led_id);
ssize_t attr_write_breath_effect_common(struct device *dev, struct device_attribute *attr, const char *buf, size_t count, unsigned char led_id);
ssize_t attr_write_brightness_common(struct device *dev, struct device_attribute *attr, const char *buf, size_t count, unsigned char led_id);

#endif /* SET_LED_H */