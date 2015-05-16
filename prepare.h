/*
 * prepare.h
 *
 *  Created on: May 15, 2015
 *      Author: brandon
 */

#ifndef PREPARE_H_
#define PREPARE_H_

#include <stdbool.h>
#include "parsecmd.h"


bool preprared_to_exec(cmd *command);
bool redirects_ready(cmd *command);



#endif /* PREPARE_H_ */
