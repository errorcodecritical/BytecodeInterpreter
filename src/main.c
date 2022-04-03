#include <stdio.h>
#include <stdarg.h>

#include "bytecode.h"
#include "parser.h"

// bytecode32b: |OPCD|REGA|REGB|REGC|XXXX|XXXX|XXXX|XXXX|

void printRegisters(int registers[]) {
    for (int i = 0; i < 16; i++) {
        printf("$r%.2d = %d\n", i, registers[i]);
    }
    printf("\n");
}

void printMemory(int memory[]) {
    for (int i = 0; i < 10; i++) {
        printf("stack[%d] = %d\n", i, memory[i]);
    }
    printf("\n");
}

int execute(bytecode program[]);

char source[] = 
"@var .word 1234            "
"@sexy .word 69             "

"ldr $1 @sexy               "

"@loop                      "
"add $2 $2 $0 1             "
"jle $2 $1 $0 @loop         "
"str $2 @var                ";

int main() {
    compilation_unit peeb = compileFromSource(source);
    execute(peeb.executable);

    return 0;
}

int execute(bytecode program[]) {
    int registers[16] = {0};
    int memory[20] = {0};

    int opcode = 0;
    int reg_a = 0;
    int reg_b = 0;
    int reg_c = 0;
    int immediate = 0;

    while (registers[REG_COUNTER] < 8) {
        bytecode code = program[registers[REG_COUNTER]++];

        decode(code, opcode, reg_a, reg_b, reg_c, immediate);

        switch (opcode) {
        case OP_ADD:
            registers[reg_a] = registers[reg_b] + (registers[reg_c] + immediate);
            break;    
        case OP_SUB:
            registers[reg_a] = registers[reg_b] - (registers[reg_c] + immediate);
            break; 
        case OP_MUL:
            registers[reg_a] = registers[reg_b] * (registers[reg_c] + immediate);
            break; 
        case OP_DIV:
            registers[reg_a] = registers[reg_b] / (registers[reg_c] + immediate);
            break;
        case OP_AND:
            registers[reg_a] = registers[reg_b] & (registers[reg_c] + immediate);
            break;
        case OP_ORR:
            registers[reg_a] = registers[reg_b] | (registers[reg_c] + immediate);
            break; 
        case OP_SHL:
            registers[reg_a] = registers[reg_b] << (registers[reg_c] + immediate);
            break;
        case OP_SHR:
            registers[reg_a] = registers[reg_b] >> (registers[reg_c] + immediate);
            break;
        case OP_JEQ:
            if (registers[reg_a] == registers[reg_b]) {
                registers[REG_COUNTER] = (registers[reg_c] + immediate);
            }
            break;
        case OP_JNE:
            if (registers[reg_a] != registers[reg_b]) {
                registers[REG_COUNTER] = (registers[reg_c] + immediate);
            }
            break;
        case OP_JGT:
            if (registers[reg_a] > registers[reg_b]) {
                registers[REG_COUNTER] = (registers[reg_c] + immediate);
            }
            break;
        case OP_JLT:
            if (registers[reg_a] < registers[reg_b]) {
                registers[REG_COUNTER] = (registers[reg_c] + immediate);
            }
            break;
        case OP_JGE:
            if (registers[reg_a] >= registers[reg_b]) {
                registers[REG_COUNTER] = (registers[reg_c] + immediate);
            }
            break;
        case OP_JLE:
            if (registers[reg_a] <= registers[reg_b]) {
                registers[REG_COUNTER] = (registers[reg_c] + immediate);
            }
            break;
        case OP_STR:
            memory[registers[reg_c] + immediate] = registers[reg_a];
            break;
        case OP_LDR:
            registers[reg_a] = memory[registers[reg_c] + immediate];  
            break;
        }

        printRegisters(registers);
    }

    printMemory(memory);

    return 0;
}