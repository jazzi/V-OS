/*
 * Copyright 2001-2011, Haiku.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Ingo Weinhold (bonefish@users.sf.net)
 */

//!	Global library initialization/termination routines.


#include <stdio.h>
#include <stdlib.h>

#include <LooperList.h>
#include <MessagePrivate.h>
#include <RosterPrivate.h>
#include <TokenSpace.h>

#include <pthread.h>


extern void __initialize_locale_kit();


// debugging
//#define DBG(x) x
#define DBG(x)
#define OUT	printf


static void
initialize_forked_child()
{
	DBG(OUT("initialize_forked_child()\n"));

	BMessage::Private::StaticReInitForkedChild();
	BPrivate::gLooperList.InitAfterFork();
	BPrivate::gDefaultTokens.InitAfterFork();

	DBG(OUT("initialize_forked_child() done\n"));
}


static void __attribute__ ((constructor))
initialize_before()
{
	DBG(OUT("initialize_before()\n"));

	BMessage::Private::StaticInit();
	BRoster::Private::InitBeRoster();

	// TODO
	#ifdef __HAIKU__
		atfork(initialize_forked_child);	
	#else
		pthread_atfork(initialize_forked_child, NULL, NULL);
	#endif

	DBG(OUT("initialize_before() done\n"));
}


static void __attribute__ ((destructor))
initialize_after()
{
	DBG(OUT("initialize_after()\n"));

	__initialize_locale_kit();

	DBG(OUT("initialize_after() done\n"));
}


extern "C" void
terminate_after()
{
	DBG(OUT("terminate_after()\n"));

	BRoster::Private::DeleteBeRoster();
	BMessage::Private::StaticCleanup();
	BMessage::Private::StaticCacheCleanup();

	DBG(OUT("terminate_after() done\n"));
}
