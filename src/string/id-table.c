#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "string/id-table.h"
#include "math/fnv.h"
#include "math/bit.h"

void string_id_table_init(StringIdTable *st, i32 size, i64 sizeof_value)
{
    st->size = size = (i32) bit_next_power_of_two((u64) size);
    st->count = 0;
    st->sizeof_value = (i32) sizeof_value;
    st->mask = (u64) (size - 1);
    st->shift_bits = bit_highest_bit(st->mask);
    st->keys = calloc(size, sizeof *st->keys);
    st->values = malloc(size * (i32) sizeof_value);
}

void string_id_table_destroy(StringIdTable *st)
{
    free(st->keys);
    free(st->values);
}

void string_id_table_insert(StringIdTable *st, sid key, const void *value)
{
    assert(string_id_table_get(st, key) == NULL);
    assert(st->count < st->size);
    u64 mask = st->mask;
    u64 k = fnv_fold((u64) key, st->shift_bits, mask);
    sid *keys = st->keys;
    while (keys[k] > StringIdTableTombstone) {
        k = (k + 1) & mask;
    }

    keys[k] = key;
    memcpy(&st->values[k * st->sizeof_value], value, st->sizeof_value);
    st->count++;
}

void *string_id_table_get(StringIdTable *st, sid search_key)
{
    u64 mask = st->mask;
    u64 k = fnv_fold((u64) search_key, st->shift_bits, mask);
    sid *keys = st->keys;
    sid key;

    while ((key = keys[k]) > StringIdTableTombstone) {
        if (key == search_key) {
            return &st->values[k * st->sizeof_value];
        }

        k = (k + 1) & mask;
    }

    return NULL;
}

void string_id_table_delete(StringIdTable *st, sid search_key)
{
    u64 mask = st->mask;
    u64 k = fnv_fold((u64) search_key, st->shift_bits, mask);
    sid *keys = st->keys;
    sid key;

    while ((key = keys[k]) > StringIdTableTombstone) {
        if (key == search_key) {
            keys[k] = StringIdTableTombstone;
            st->count--;
            return;
        }

        k = (k + 1) & mask;
    }
}
