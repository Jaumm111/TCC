#ifndef _MAP_SUNSPEC_
#define	_MAP_SUNSPEC_

#include "string.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>


typedef enum{
    SUNSPEC_TYPE_UNKNOWN = 0,
    SUNSPEC_TYPE_SUNSSF,
    SUNSPEC_TYPE_PAD,
    SUNSPEC_TYPE_COUNT,
    SUNSPEC_TYPE_UINT16,
    SUNSPEC_TYPE_INT16,
    SUNSPEC_TYPE_ENUM16,
    SUNSPEC_TYPE_BITFIELD16,
    SUNSPEC_DIVISOR_1REG,

    SUNSPEC_TYPE_UINT32,
    SUNSPEC_TYPE_BITFIELD32,
    SUNSPEC_TYPE_FLOAT32,
    SUNSPEC_TYPE_ACC32,
    SUNSPEC_DIVISOR_2REG,

    SUNSPEC_TYPE_ACC64,
    SUNSPEC_TYPE_STRING4,
    SUNSPEC_DIVISOR_4REG,

    SUNSPEC_TYPE_STRING8,
    SUNSPEC_DIVISOR_8REG,

    SUNSPEC_TYPE_STRING16,
    SUNSPEC_DIVISOR_16REG,
}sunspec_type_t;

typedef enum{
    SUNSPEC_TABEL_1= 0,
    SUNSPEC_TABEL_101,
    SUNSPEC_TABEL_111,
}tabel_model_t;

typedef struct {
    uint32_t SID;
    uint16_t ID;
    uint16_t L;
    char Manufacturer[32];
    char Device[32];
    char Options[16];
    char Version[16];
    char Serial_Number[32];
    uint16_t addr;
} map_ID_inverter;

typedef struct{
    sunspec_type_t type;
    char *label;
    char *scale;
    char *units;
    size_t size;
}table_sunspec;

typedef struct {
    table_sunspec *table;
    size_t count;
    size_t num_reg;
} map_inverter_full;

typedef struct {
    uint16_t ID;
    uint16_t L;
} id_and_l;



char *convert_map_to_string(void *, tabel_model_t );

//char *get_ptr_json();
//uint32_t get_tam_ptr_json();


#endif /* _MAP_ */