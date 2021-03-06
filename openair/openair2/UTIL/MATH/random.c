/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

/*
                                 random.c
                             -------------------
  AUTHOR  : Lionel GAUTHIER
  COMPANY : EURECOM
  EMAIL   : Lionel.Gauthier@eurecom.fr
 ***************************************************************************/
#include "rtos_header.h"
#include "platform_types.h"

#ifdef USER_MODE
#    include <sys/time.h>
#else
#include <rtai_sched.h>
#endif


/* Random generators */
#define FACTOR       16807
#define LASTXN       127773
#define UPTOMOD     -2836

static int      seed;


void
init_uniform (void)
{
#ifdef USER_MODE
  struct timeval  tv;
  struct timezone tz;

  gettimeofday (&tv, &tz);
  seed = (int) tv.tv_usec;
#ifdef NODE_MT
#warning TO DO seed = mobileId
  //seed += mobileId;
#endif
#ifdef NODE_RG
#warning TO DO seed = rgId
  //seed += rgId;
#endif
#else
  seed = rt_get_time_ns();
#endif
}


int
uniform (void)
{
  static int      times, rest, prod1, prod2;

  times = seed / LASTXN;
  rest = seed - times * LASTXN;
  prod1 = times * UPTOMOD;
  prod2 = rest * FACTOR;
  seed = prod1 + prod2;

  return seed;
}
