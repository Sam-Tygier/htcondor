////////////////////////////////////////////////////////////////////////////////
//
// condor_sched.h
//
// Define class Scheduler. This class does local scheduling and then negotiates
// with the central manager to obtain resources for jobs.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _CONDOR_SCHED_H_
#define _CONDOR_SCHED_H_

#include "_condor_fix_types.h"
#include "condor_classad.h"
#include "condor_io.h"
#include "proc.h"
#include "sched.h"
#include "prio_rec.h"
#include "HashTable.h"
#include "list.h"
#include "classad_hashtable.h"	// for HashKey class
#include "Queue.h"

const 	int			MAX_NUM_OWNERS = 512;
const 	int			MAX_REJECTED_CLUSTERS = 1024;


extern	char**		environ;

struct shadow_rec
{
    int             pid;
    PROC_ID         job_id;
    struct match_rec*    match;
    int             preempted;
    int             conn_fd;
	int				removed;
};

struct OwnerData {
  char* Name;
  int JobsRunning;
  int JobsIdle;
  OwnerData() { Name=NULL; JobsRunning=JobsIdle=0; }
};

struct match_rec
{
    match_rec(char*, char*, PROC_ID*);
    char    		id[SIZE_OF_CAPABILITY_STRING];
    char    		peer[50];
    int     		cluster;
    int     		proc;
    int     		status;
    int     		agentPid;
	shadow_rec*		shadowRec;
	int				alive_countdown;
};

enum
{
    M_ACTIVE,
    M_INACTIVE,
    M_DELETED
};

class Scheduler : public Service
{
  public:
	
	Scheduler();
	~Scheduler();
	
	// initialization
	void			Init();
	void			Register();
	void			RegisterTimers();

	// maintainence
	void			timeout(); 
	void			reconfig();
	void			shutdown_fast();
	void			shutdown_graceful();
	
	// negotiation
	int				doNegotiate(int, Stream *);
	int				negotiatorSocketHandler(Stream *);
	int				negotiate(int, Stream *);
	void			reschedule_negotiator(int, Stream *);
	void			vacate_service(int, Stream *);

	// job managing
	int				abort_job(int, Stream *);
	void			send_all_jobs(ReliSock*, struct sockaddr_in*);
	void			send_all_jobs_prioritized(ReliSock*, struct sockaddr_in*);
	friend	int		count(ClassAd *);
	friend	void	job_prio(ClassAd *);
	friend  int		find_idle_sched_universe_jobs(ClassAd *);
	void			display_shadow_recs();

	// match managing
    match_rec*       	AddMrec(char*, char*, PROC_ID*);
    int         	DelMrec(char*);
    int         	DelMrec(match_rec*);
    int         	MarkDel(char*);
    match_rec*       	FindMrecByPid(int);
	shadow_rec*		FindSrecByPid(int);
	shadow_rec*		FindSrecByProcID(PROC_ID);
	void			RemoveShadowRecFromMrec(shadow_rec*);
	int				AlreadyMatched(PROC_ID*);
	void			Agent(char*, char*, char*, char*, int, ClassAd*);
	void			StartJobs();
	void			StartSchedUniverseJobs();
	void			send_alive();
	void			StartJobHandler();
	
  private:
	
	// information about this scheduler
	ClassAd*		ad;
	char*			MySockName;		// dhaval
	Scheduler*		myself;
	
	// information for utilizing cached negotiator socket
	bool			alreadyStashed;

	// parameters controling the scheduling and starting shadow
	int				SchedDInterval;
	int				QueueCleanInterval;
	int				JobStartDelay;
	int				MaxJobsRunning;
	int				JobsStarted; // # of jobs started last negotiating session
	int				SwapSpace;	 // available at beginning of last session
	int				ShadowSizeEstimate;	// Estimate of swap needed to run a job
	int				SwapSpaceExhausted;	// True if job died due to lack of swap space
	int				ReservedSwap;		// for non-condor users
	int				JobsIdle; 
	int				JobsRunning;
	int				SchedUniverseJobsIdle;
	int				SchedUniverseJobsRunning;
	int				BadCluster;
	int				BadProc;
	int				RejectedClusters[MAX_REJECTED_CLUSTERS];
	int				N_RejectedClusters;
    OwnerData			Owners[MAX_NUM_OWNERS];
	int				N_Owners;
	time_t			LastTimeout;
	int				ExitWhenDone;  // Flag set for graceful shutdown
	Queue<shadow_rec*> RunnableJobQueue;
	int				StartJobTimer;
	
	// useful names
	char*			CondorViewHost;
	char*			CollectorHost;
	char*			NegotiatorHost;
	char*			Shadow;
	char*			CondorAdministrator;
	char*			Mail;
	char*			filename;					// save UpDown object
	char*			AccountantName;
    char*                   UidDomain;

	// connection variables
	struct sockaddr_in	From;
	int					Len; 

	// utility functions
	int				shadow_prio_recs_consistent();
	void			mail_problem_message();
	int				cluster_rejected(int);
	void   			mark_cluster_rejected(int); 
	int				count_jobs();
	void			update_central_mgr(int command);
	int				insert_owner(char*);
	void			reaper(int, int, struct sigcontext*);
	void			clean_shadow_recs();
	void			preempt(int);
	int				permission(char*, char*, char*, PROC_ID*);
	shadow_rec*		StartJob(match_rec*, PROC_ID*);
	shadow_rec*		start_std(match_rec*, PROC_ID*);
	shadow_rec*		start_pvm(match_rec*, PROC_ID*);
	shadow_rec*		start_sched_universe_job(PROC_ID*);
	void			Relinquish(match_rec*);
	void 			swap_space_exhausted();
	void			delete_shadow_rec(int);
	void			mark_job_running(PROC_ID*);
	int				is_alive(int);
	void			check_zombie(int, PROC_ID*);
	void			kill_zombie(int, PROC_ID*);
	shadow_rec*     find_shadow_rec(PROC_ID*);
	shadow_rec*     add_shadow_rec(int, PROC_ID*, match_rec*, int);
	shadow_rec*		add_shadow_rec(shadow_rec*);
	void			NotifyUser(shadow_rec*, char*, int, int);
	
#ifdef CARMI_OPS
	shadow_rec*		find_shadow_by_cluster( PROC_ID * );
#endif

	HashTable <HashKey, match_rec *> *matches;
	HashTable <int, shadow_rec *> *shadowsByPid;
	HashTable <PROC_ID, shadow_rec *> *shadowsByProcID;
	int				numMatches;
	int				numShadows;
	List <PROC_ID>	*IdleSchedUniverseJobIDs;
    int         	aliveInterval;             // how often to broadcast alive
};
	
#endif
