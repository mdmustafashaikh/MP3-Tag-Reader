#ifndef TYPES_H
#define TYPES_H

/* User defined types */
typedef unsigned int uint;

/* Status enumeration to represent the success or failure of operations.
 * e_success indicates a successful operation, while e_failure indicates an error.
 */

typedef enum
{
    e_success,
    e_failure
} Status;

 typedef enum
{
    e_view,
    e_help,
    e_edit
} OperationType;

#endif
