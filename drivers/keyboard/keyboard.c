#include "keyboard.h"
#include "../../kernel/kstatus.h"
#include "../../kernel/task/process/process.h"
#include "ps2/ps2.h"


static struct keyboard* keyboard_list_head = 0;
static struct keyboard* keyboard_list_tail = 0;


int keyboard_add(struct keyboard* _keyboard){

    if(_keyboard->init == 0){
        return -ERR_INVARG;
    }

    if(keyboard_list_tail){
        keyboard_list_tail->next = _keyboard;
        keyboard_list_tail = keyboard_list_tail->next;
    }else{
        keyboard_list_head = _keyboard;
        keyboard_list_tail = _keyboard;
    }

    return _keyboard->init();

}

void keyboard_init(){
#if defined(PS2)
    keyboard_add(ps2_init());
#endif
}

static void keyboard_pop_from_buffer(struct process* _process){

    _process->keyboard_buf.tail--;
    _process->keyboard_buf.buffer[_process->keyboard_buf.tail % PROCESS_MAX_KEYBOARD_BUFFER_SIZE] = 0;

}

static void keyboard_push_to_buffer(struct process* _process, char ch){

    uint32_t index_to_push = _process->keyboard_buf.tail % PROCESS_MAX_KEYBOARD_BUFFER_SIZE;
    _process->keyboard_buf.buffer[index_to_push] = ch;
    _process->keyboard_buf.tail++;
}

void keyboard_put(char ch){
    if(!process_current()) return;

    keyboard_push_to_buffer(process_current(), ch);

}

void keyboard_backspace(){
    if(!process_current()) return;

    keyboard_pop_from_buffer(process_current());
}