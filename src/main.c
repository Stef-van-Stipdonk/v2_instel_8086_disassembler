#include "define.h"
#include <assert.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char nodisp[8][10] = {{"bx + si"}, {"bx + di"}, {"bp + si"}, {"bp + di"},
                      {"si"},      {"di"},      {"bp"},      {"bx"}};

const char *reg_byte[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
const char *reg_word[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};

s16 reg_values[8] = {0};

const u8 *read_file(const char *path, size_t *out_size) {
  FILE *f = fopen(path, "rb");
  assert(f != NULL);
  assert(fseek(f, 0, SEEK_END) == 0);
  size_t size = ftell(f);
  assert(fseek(f, 0, SEEK_SET) == 0);
  u8 *data = malloc(size);
  assert(fread(data, size, 1, f) == 1);
  if (out_size)
    *out_size = size;
  fclose(f);
  return data;
}

static void print_byte_as_binary(const u8 *byte, u8 amount) {
  for (int i = 0; i < amount; i++) {
    for (int y = 7; y >= 0; y--) {
      u8 op = byte[i];
      printf("%d", (op >> y) & 1);
    }
  }

  printf("\n");
}

void register_or_memory_to_from_register(const u8 *data) {}

void immediate_to_register_or_memory(const u8 *data) {}

void immediate_to_accumulator(const u8 *data) {}

void disasm(const u8 *data, const size_t size,
            const bool should_simulate_execution) {
  const u8 *data_end = data + size;

  while (data < data_end) {
    u8 byte0 = *data;
    switch ((byte0 >> 2)) {
    case 0b100010: // mov register <-> register/memory
      register_or_memory_to_from_register(data);
      break;
    case 0b000000: // add register <-> register/memory
      register_or_memory_to_from_register(data);
      break;
    case 0b001010: // sub register <-> register/memory
      register_or_memory_to_from_register(data);
      break;
    case 0b001110: // cmp register <-> register/memory
      register_or_memory_to_from_register(data);
      break;
    case 0b100000: // add, sub, cmp register/memory <- immediate
      immediate_to_register_or_memory(data);
      break;
    case 0b000001: // add immediate > accumulator
      immediate_to_accumulator(data);
      break;
    case 0b001011: // sub immediate > accumulator
      immediate_to_accumulator(data);
      break;
    case 0b101000: // mov immediate > accumulator
      immediate_to_accumulator(data);
      break;
    case 0b001111: // cmp immediate > accumulator
      immediate_to_accumulator(data);
      break;
    default:
      switch ((byte0 >> 1)) {
      case 0b1100011: // mov register/memory <- immediate
        break;
      }

      switch ((byte0 >> 4)) {
      case 0b1011: // mov register <- immediate
        break;
      }

      printf("Unknown opcode ");
      print_byte_as_binary(data, 1);
      exit(EXIT_FAILURE);
    }

    printf("\n");
  }
}

int main(int argc, char **args) {

  u8 file_pos = 1;
  bool should_simulate_execution = false;

  if (argc > 2) {
    assert(args[2]);
    if (strcmp(args[1], "--exec") == 0 || strcmp(args[1], "-e") == 0) {
      file_pos = 2;
      should_simulate_execution = true;
    }
  }

  size_t size;
  const u8 *data = read_file(args[file_pos], &size);

  printf("; disassembly of %s, %zdb\n", args[file_pos], size);
  printf("bits 16\n");
  disasm(data, size, should_simulate_execution);
  if (should_simulate_execution) {
    printf("\nFinal registers:\n");
    printf("\t %s: 0x%.4x (%d)\n", reg_word[0], reg_values[0], reg_values[0]);
    printf("\t %s: 0x%.4x (%d)\n", reg_word[1], reg_values[1], reg_values[1]);

    printf("\t %s: 0x%.4x (%d)\n", reg_word[2], reg_values[2], reg_values[2]);
    printf("\t %s: 0x%.4x (%d)\n", reg_word[3], reg_values[3], reg_values[3]);
    printf("\t %s: 0x%.4x (%d)\n", reg_word[4], reg_values[4], reg_values[4]);
    printf("\t %s: 0x%.4x (%d)\n", reg_word[5], reg_values[5], reg_values[5]);
    printf("\t %s: 0x%.4x (%d)\n", reg_word[6], reg_values[6], reg_values[6]);
    printf("\t %s: 0x%.4x (%d)\n", reg_word[7], reg_values[7], reg_values[7]);
  }
  return EXIT_SUCCESS;
}
