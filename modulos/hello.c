/**
 * @file hello.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2020-02-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <linux/module.h>
#include <linux/kernel.h>

int hello_init(void)
{
    pr_alert("Hello World :)\n");
    return 0;
}
void hello_exit(void)
{
    pr_alert("Goodbye World!\n");
}
module_init(hello_init);
module_exit(hello_exit);
