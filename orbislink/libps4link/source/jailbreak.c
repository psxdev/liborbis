#include <stdint.h>
struct auditinfo_addr {
    /*
    4   ai_auid;
    8   ai_mask;
    24  ai_termid;
    4   ai_asid;
    8   ai_flags;r
    */
    char useless[184];
};

struct ucred {
    uint32_t useless1;
    uint32_t cr_uid;     // effective user id
    uint32_t cr_ruid;    // real user id
    uint32_t useless2;
    uint32_t useless3;
    uint32_t cr_rgid;    // real group id
    uint32_t useless4;
    void *useless5;
    void *useless6;
    void *cr_prison;     // jail(2)
    void *useless7;
    uint32_t useless8;
    void *useless9[2];
    void *useless10;
    struct auditinfo_addr useless11;
    uint32_t *cr_groups; // groups
    uint32_t useless12;
};

struct proc {
    char useless[64];
    struct ucred *p_ucred;
};

struct thread {
    void *useless;
    struct proc *td_proc;
};

struct fileops {
    void    *fo_read;
    void    *fo_write;
    void    *fo_truncate;
    void    *fo_ioctl;
    void    *fo_poll;
    void    *fo_kqfilter;
    void    *fo_stat;
    void    *fo_close;
    void    *fo_chmod;
    void    *fo_chown;
    int fo_flags;   /* DFLAG_* below */
};


int jailbreak(struct thread *td, void *uap) {
    struct ucred *cred;

	// Disable write protection
	ps4KernelProtectionWriteDisable();
	 //sysctl_machdep_rcmgr_debug_menu and sysctl_machdep_rcmgr_store_moe
	//*(uint16_t *)0xFFFFFFFF82607C46 = 0x9090;
	//*(uint16_t *)0xFFFFFFFF82607826 = 0x9090;
	
	//*(char *)0xFFFFFFFF8332431A = 1;
	//*(char *)0xFFFFFFFF83324338 = 1;
	
	//dex
	//*(char *)0xFFFFFFFF833242FD = 0x81;
	
	//Fully enable Debug Settings: 
	//*(uint32_t*) 0xFFFFFFFF8269C07C=0; 
	//*(uint32_t*) 0xFFFFFFFF8269B56F=0;
	
	//spoof
	//((uint64_t *) 0xFFFFFFFF8323A4E0)[0] = 0x9000000;
	// Disable Process ASLR - Thanks to ZiL0G80
	*(uint16_t *)0xFFFFFFFF82649C9C = 0x63EB;
	
	// Restore write protection
	ps4KernelProtectionWriteEnable();
	// Resolve creds
    cred = td->td_proc->p_ucred;

    // Escalate process to root
    cred->cr_uid = 0;
    cred->cr_ruid = 0;
    cred->cr_rgid = 0;
    cred->cr_groups[0] = 0;

    void *td_ucred = *(void **)(((char *)td) + 304); // p_ucred == td_ucred

    // sceSblACMgrIsSystemUcred
    uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
    *sonyCred = 0xffffffffffffffffULL;

    // sceSblACMgrGetDeviceAccessType
    uint64_t *sceProcType = (uint64_t *)(((char *)td_ucred) + 88);
    *sceProcType = 0x3801000000000013ULL; // Max access

    // sceSblACMgrHasSceProcessCapability
    uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
    *sceProcCap = 0xffffffffffffffffULL; // Sce Process
	

    ((uint64_t *)0xFFFFFFFF832CC2E8ULL)[0] = 0x123456; //priv_check_cred bypass with suser_enabled=true
    ((uint64_t *)0xFFFFFFFF8323DA18ULL)[0] = 0; // bypass priv_check

    // Jailbreak ;)
    cred->cr_prison = (void *)0xFFFFFFFF83237250ULL; //&prison0

    // Break out of the sandbox
    void *td_fdp = *(void **)(((char *)td->td_proc) + 72);
    uint64_t *td_fdp_fd_rdir = (uint64_t *)(((char *)td_fdp) + 24);
    uint64_t *td_fdp_fd_jdir = (uint64_t *)(((char *)td_fdp) + 32);
    uint64_t *rootvnode = (uint64_t *)0xFFFFFFFF832EF920ULL;
    *td_fdp_fd_rdir = *rootvnode;
    *td_fdp_fd_jdir = *rootvnode;

    return 0;
}
