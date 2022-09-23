/**
 * Copyright (c) 2022 Fábián Varga
 *
 * :: The Pico Logic Emulator project ::
 * 
 * This project will turn a Pico board into a multi-functional
 * logic gate.
 * 
 * Emulated logid gates:
 * -----------------------------
 * AND:
 *      -> Input #1: GPIO #5, Input #2: GPIO #6
 *      <- Output: GPIO #7
 *      <! Inverted output: GPIO #8
 * OR:
 *      -> Input #1: GPIO #9, Input #2: GPIO #10
 *      <- Output: GPIO #11
 *      <! Inverted output: GPIO #12
 * XOR:
 *      -> Input #1: GPIO #13, Input #2: GPIO #14
 *      <- Output: GPIO #15
 *      <! Inverted output: GPIO #16
 * Function: !((A + B) * !A)
 *      -> Input #1: GPIO #17, Input #2: GPIO #18
 *      <- Output: GPIO #19
 */

#include "pico/stdlib.h"
#include "pinconfig.h"

//Optionally pull the inputs high or low
//Note: Not required, both can be undefined.
//#define FORCE_PULL_UP
#define FORCE_PULL_DOWN

bool compute_and_gate_result() { return gpio_get(AND_GATE_IN_A)  &&  gpio_get(AND_GATE_IN_B);  }
bool compute_or_gate_result()  { return gpio_get(OR_GATE_IN_A)   ||  gpio_get(OR_GATE_IN_B);   }
bool compute_xor_gate_result() { return !gpio_get(XOR_GATE_IN_A) != !gpio_get(XOR_GATE_IN_B);  }

bool process_func_result() {
    //Formula: !((A + B) * !A)
    bool A = compute_xor_gate_result();
    bool B = compute_xor_gate_result();
    return !(
        // ( A + B ) -- OR
        (A || B)
        // * !A      -- AND + INV
        && (!A)
    );
}

void process_and_gate(){
    bool x = compute_and_gate_result();
    gpio_put(AND_GATE_OUT, x);
    gpio_put(AND_GATE_IOUT, !x);
}

void process_or_gate(){
    bool x = compute_or_gate_result();
    gpio_put(OR_GATE_OUT, x);
    gpio_put(OR_GATE_IOUT, !x);
}

void process_xor_gate(){
    bool x = compute_xor_gate_result();
    gpio_put(XOR_GATE_OUT, x);
    gpio_put(XOR_GATE_IOUT, !x);
}

int main() {
    //Set up pins
    gpio_init(AND_GATE_IN_A); gpio_set_dir(AND_GATE_IN_A, GPIO_IN);
    gpio_init(AND_GATE_IN_B); gpio_set_dir(AND_GATE_IN_B, GPIO_IN);
    gpio_init(AND_GATE_OUT);  gpio_set_dir(AND_GATE_OUT, GPIO_OUT);
    gpio_init(AND_GATE_IOUT); gpio_set_dir(AND_GATE_IOUT, GPIO_OUT);

    gpio_init(OR_GATE_IN_A); gpio_set_dir(OR_GATE_IN_A, GPIO_IN);
    gpio_init(OR_GATE_IN_B); gpio_set_dir(OR_GATE_IN_B, GPIO_IN);
    gpio_init(OR_GATE_OUT);  gpio_set_dir(OR_GATE_OUT, GPIO_OUT);
    gpio_init(OR_GATE_IOUT); gpio_set_dir(OR_GATE_IOUT, GPIO_OUT);

    gpio_init(XOR_GATE_IN_A); gpio_set_dir(XOR_GATE_IN_A, GPIO_IN);
    gpio_init(XOR_GATE_IN_B); gpio_set_dir(XOR_GATE_IN_B, GPIO_IN);
    gpio_init(XOR_GATE_OUT);  gpio_set_dir(XOR_GATE_OUT, GPIO_OUT);
    gpio_init(XOR_GATE_IOUT); gpio_set_dir(XOR_GATE_IOUT, GPIO_OUT);

    gpio_init(FUNC_IN_A); gpio_set_dir(FUNC_IN_A, GPIO_IN);
    gpio_init(FUNC_IN_B); gpio_set_dir(FUNC_IN_B, GPIO_IN);
    gpio_init(FUNC_OUT);  gpio_set_dir(FUNC_OUT, GPIO_OUT);

    // Pull the inputs high or low if requested
    #ifdef FORCE_PULL_DOWN
        gpio_pull_down(AND_GATE_IN_A);
        gpio_pull_down(AND_GATE_IN_B);
        gpio_pull_down(OR_GATE_IN_A);
        gpio_pull_down(OR_GATE_IN_B);
        gpio_pull_down(XOR_GATE_IN_A);
        gpio_pull_down(XOR_GATE_IN_B);
    #elif defined(FORCE_PULL_UP)
        gpio_pull_up(AND_GATE_IN_A);
        gpio_pull_up(AND_GATE_IN_B);
        gpio_pull_up(OR_GATE_IN_A);
        gpio_pull_up(OR_GATE_IN_B);
        gpio_pull_up(XOR_GATE_IN_A);
        gpio_pull_up(XOR_GATE_IN_B);
    #endif
    

    // Process inputs and set outputs accordingly
    while(true){
        process_and_gate();
        process_or_gate();
        process_xor_gate();
        process_func_result();
    }

    return 0;
}