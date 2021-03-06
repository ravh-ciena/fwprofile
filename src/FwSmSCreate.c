/**
 * @file
 * @ingroup smGroup
 * Implements the static initialization functions for the FW
 * State Machine Module.
 * @author Vaclav Cechticky <vaclav.cechticky@pnp-software.com>
 * @author Alessandro Pasetti <pasetti@pnp-software.com>
 * @copyright P&P Software GmbH, 2011, All Rights Reserved
 *
 * This file is part of the FW Profile.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * For information on alternative licensing, please contact P&P Software GmbH.
 */

#include "FwSmSCreate.h"
#include "FwSmPrivate.h"
#include <stdlib.h>

/* ----------------------------------------------------------------------------------------------------------------- */
void FwSmInit(FwSmDesc_t smDesc) {

  FwSmCounterS1_t i;
  SmBaseDesc_t*   smBase = smDesc->smBase;

  for (i = 0; i < smBase->nOfPStates; i++) {
    smBase->pStates[i].outTransIndex = 0;
    smDesc->esmDesc[i]               = NULL;
  }

  for (i = 0; i < smBase->nOfCStates; i++) {
    smBase->cStates[i].outTransIndex = 0;
  }

  for (i = 0; i < smBase->nOfTrans; i++) {
    smBase->trans[i].iTrAction = -1;
  }

  smDesc->smActions[0] = &SmDummyAction;
  for (i = 1; i < smDesc->nOfActions; i++) {
    smDesc->smActions[i] = NULL;
  }

  smDesc->smGuards[0] = &SmDummyGuard;
  for (i = 1; i < smDesc->nOfGuards; i++) {
    smDesc->smGuards[i] = NULL;
  }
}

/* ----------------------------------------------------------------------------------------------------------------- */
void FwSmInitDer(FwSmDesc_t smDesc, FwSmDesc_t smDescBase) {
  FwSmCounterS1_t i;
  SmBaseDesc_t*   smBase = smDescBase->smBase;

  if (smDesc->nOfActions != smDescBase->nOfActions) {
    smDesc->errCode = smWrongNOfActions;
    return;
  }

  if (smDesc->nOfGuards != smDescBase->nOfGuards) {
    smDesc->errCode = smWrongNOfGuards;
    return;
  }

  smDesc->smBase = smBase;

  /* This cycle will always be executed at least once because
   * the number of actions (nOfActions) is always greater than
   * zero (since all state machines have at least the dummy
   * action which returns without doing anything)
   */
  for (i = 0; i < smDesc->nOfActions; i++) {
    smDesc->smActions[i] = smDescBase->smActions[i];
  }

  /* This cycle will always be executed at least once because
   * the number of guards (nOfGuards) is always greater than
   * zero (since all state machines have at least the dummy
   * guard which always returns TRUE)
   */
  for (i = 0; i < smDesc->nOfGuards; i++) {
    smDesc->smGuards[i] = smDescBase->smGuards[i];
  }

  for (i = 0; i < smBase->nOfPStates; i++) {
    smDesc->esmDesc[i] = NULL;
  }

  smDesc->errCode      = smDescBase->errCode;
  smDesc->smExecCnt    = 0;
  smDesc->stateExecCnt = 0;
  smDesc->transCnt     = 0;
  smDesc->curState     = 0;

  return;
}
