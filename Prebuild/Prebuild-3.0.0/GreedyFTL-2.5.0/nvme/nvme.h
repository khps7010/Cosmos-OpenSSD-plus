//////////////////////////////////////////////////////////////////////////////////
// nvme.h for Cosmos OpenSSD
// Copyright (c) 2016 Hanyang University ENC Lab.
// Contributed by Yong Ho Song <yhsong@enc.hanyang.ac.kr>
//				  Youngjin Jo <yjjo@enc.hanyang.ac.kr>
//				  Sangjin Lee <sjlee@enc.hanyang.ac.kr>
//
// This file is part of Cosmos OpenSSD.
//
// Cosmos OpenSSD is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3, or (at your option)
// any later version.
//
// Cosmos OpenSSD is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Cosmos OpenSSD; see the file COPYING.
// If not, see <http://www.gnu.org/licenses/>.
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Company: ENC Lab. <http://enc.hanyang.ac.kr>
// Engineer: Sangjin Lee <sjlee@enc.hanyang.ac.kr>
//
// Project Name: Cosmos OpenSSD
// Design Name: Cosmos Firmware
// Module Name: NVMe header
// File Name: nvme.h
//
// Version: v1.0.0
//
// Description:
//   - defines parameters and data structures of the NVMe controller
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// Revision History:
//
// * v1.0.0
//   - First draft
//////////////////////////////////////////////////////////////////////////////////

#ifndef __NVME_H_
#define __NVME_H_

#define MAX_NUM_OF_IO_SQ	8
#define MAX_NUM_OF_IO_CQ	8

#define ADMIN_CMD_DRAM_DATA_BUFFER		0x01000000
#define IO_DRAM_DATA_BUFFER				0x10000000

#define STORAGE_CAPACITY_L				0x00000000	// not used
#define STORAGE_CAPACITY_H				0x00000000

#define MAX_NUM_OF_NLB					(512 * 1024 / 4096)

/*Opcodes for Admin Commands */
#define ADMIN_DELETE_IO_SQ									0x00
#define ADMIN_CREATE_IO_SQ									0x01
#define ADMIN_GET_LOG_PAGE									0x02
#define ADMIN_DELETE_IO_CQ									0x04
#define ADMIN_CREATE_IO_CQ									0x05
#define ADMIN_IDENTIFY										0x06
#define ADMIN_ABORT											0x08
#define ADMIN_SET_FEATURES									0x09
#define ADMIN_GET_FEATURES									0x0A
#define ADMIN_ASYNCHRONOUS_EVENT_REQUEST					0x0C
#define ADMIN_FIRMWARE_ACTIVATE								0x10
#define ADMIN_FIRMWARE_IMAGE_DOWNLOAD						0x11
#define ADMIN_FORMAT_NVM									0x80
#define ADMIN_SECURITY_SEND									0x81
#define ADMIN_SECURITY_RECEIVE								0x82

/*Opcodes for IO Commands */
#define IO_NVM_FLUSH										0x00
#define IO_NVM_WRITE										0x01
#define IO_NVM_READ											0x02
#define IO_NVM_WRITE_UNCORRECTABLE							0x04
#define IO_NVM_COMPARE										0x05
#define IO_NVM_DATASET_MANAGEMENT							0x09


/*Status Code Type */
#define GENERIC_COMMAND_STATUS								0
#define COMMAND_SPECIFIC_ERRORS								1
#define MEDIA_ERRORS										2

/*Status Code - Generic Command Status Values */
#define SUCCESSFUL_COMPLETION								0x0
#define INVALID_COMMAND_OPCODE								0x1
#define INVALID_FIELD_IN_COMMAND							0x2
#define COMMAND_ID_CONFLICT									0x3
#define DATA_TRANSFER_ERROR									0x4
#define COMMANDS_ABORTED_DUE_TO_POWER_LOSS_NOTIFICATION		0x5
#define INTERNAL_DEVICE_ERROR								0x6
#define COMMAND_ABORT_REQUESTED								0x7
#define COMMAND_ABORTED_DUE_TO_SQ_DELETION					0x8
#define COMMAND_ABORTED_DUE_TO_FAILED_FUSED_COMMAND			0x9
#define COMMAND_ABORTED_DUE_TO_MISSING_FUSED_COMMAND		0xA
#define INVALID_NAMESPACE_OR_FORMAT							0xB


/* Set/Get Features - Features Identifiers */

#define ARBITRATION											0x01
#define POWER_MANAGEMENT									0x02
#define LBA_RANGE_TYPE										0x03
#define TEMPERATURE_THRESHOLD								0x04
#define ERROR_RECOVERY										0x05
#define VOLATILE_WRITE_CACHE								0x06
#define NUMBER_OF_QUEUES									0x07
#define INTERRUPT_COALESCING								0x08
#define INTERRUPT_VECTOR_CONFIGURATION						0x09
#define WRITE_ATOMICITY										0x0A
#define ASYNCHRONOUS_EVENT_CONFIGURATION					0x0B
#define SOFTWARE_PROGRESS_MARKER							0x80


#define NVME_TASK_IDLE										0x0
#define NVME_TASK_WAIT_CC_EN								0x1
#define NVME_TASK_RUNNING									0x2
#define NVME_TASK_SHUTDOWN									0x3
#define NVME_TASK_WAIT_RESET								0x4
#define NVME_TASK_RESET										0x5
#pragma pack(push, 1)

typedef struct _NVME_COMMAND
{
	unsigned short qID;
	unsigned short cmdSlotTag;
	unsigned int cmdSeqNum;
	unsigned int cmdDword[16];
}NVME_COMMAND;

typedef struct _NVME_ADMIN_COMMAND
{
	union {
		unsigned int dword[16];
		struct {
			struct {
				unsigned char OPC;
				unsigned char FUSE			:2;
				unsigned char reserved0		:5;
				unsigned char PSDT			:1;
				unsigned short CID;
			};
			unsigned int NSID;
			unsigned int reserved1[2];
			unsigned int MPTR[2];
			unsigned int PRP1[2];
			unsigned int PRP2[2];
			unsigned int dword10;
			unsigned int dword11;
			unsigned int dword12;
			unsigned int dword13;
			unsigned int dword14;
			unsigned int dword15;
		};
	};
}NVME_ADMIN_COMMAND;

typedef struct _NVME_IO_COMMAND
{
	union {
		unsigned int dword[16];
		struct {
			struct {
				unsigned char OPC;
				unsigned char FUSE			:2;
				unsigned char reserved0		:5;
				unsigned char PSDT			:1;
				unsigned short CID;
			};
			unsigned int NSID;
			unsigned int reserved1[2];
			unsigned int MPTR[2];
			unsigned int PRP1[2];
			unsigned int PRP2[2];
			unsigned int dword10;
			unsigned int dword11;
			unsigned int dword12;
			unsigned int dword13;
			unsigned int dword14;
			unsigned int dword15;
		};
	};
}NVME_IO_COMMAND;

typedef struct _NVME_COMPLETION
{
	union {
		unsigned int dword[2];
		struct {
			union {
				unsigned short statusFieldWord;
				struct 
				{
					unsigned short reserved0		:1;
					unsigned short SC				:8;
					unsigned short SCT				:3;
					unsigned short reserved1		:2;
					unsigned short MORE				:1;
					unsigned short DNR				:1;
				}statusField;
			};
			
			unsigned int specific;
		};
	};
}NVME_COMPLETION;

typedef struct _ADMIN_SET_FEATURES_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned char FID;
			unsigned char reserved0[2];
			unsigned char reserved1			:7;
			unsigned char SV				:1;
		};
	};
} ADMIN_SET_FEATURES_DW10;

typedef struct _ADMIN_SET_FEATURES_NUMBER_OF_QUEUES_DW11
{
	union {
		unsigned int dword;
		struct {
			unsigned short NCQR;
			unsigned short NSQR;
		};
	};
} ADMIN_SET_FEATURES_NUMBER_OF_QUEUES_DW11;


/* Get Features Command */
typedef struct _ADMIN_GET_FEATURES_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned char FID;
			unsigned char SEL				:3;
			unsigned char reserved0			:5;
			unsigned char reserved1[2];
		};
	};
} ADMIN_GET_FEATURES_DW10;

/* Create I/O Completion Queue Command */
typedef struct _ADMIN_CREATE_IO_CQ_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned short QID;
			unsigned short QSIZE;
		};
	};
} ADMIN_CREATE_IO_CQ_DW10;

typedef struct _ADMIN_CREATE_IO_CQ_DW11
{
	union {
		unsigned int dword;
		struct {
			unsigned short PC				:1;
			unsigned short IEN				:1;
			unsigned short reserved0		:14;
			unsigned short IV;
		};
	};
} ADMIN_CREATE_IO_CQ_DW11;


/* Delete I/O Completion Queue Command */
typedef struct _ADMIN_DELETE_IO_CQ_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned short QID;
			unsigned short reserved0;
		};
	};
} ADMIN_DELETE_IO_CQ_DW10;



/* Create I/O Submission Queue Command */
typedef struct _ADMIN_CREATE_IO_SQ_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned short QID;
			unsigned short QSIZE;
		};
	};
} ADMIN_CREATE_IO_SQ_DW10;

typedef struct _ADMIN_CREATE_IO_SQ_DW11
{
	union {
		unsigned int dword;
		struct {
			unsigned short PC			:1;
			unsigned short QPRIO		:2;
			unsigned short reserved0	:13;
			unsigned short CQID;
		};
	};
} ADMIN_CREATE_IO_SQ_DW11;

/* Delete I/O Submission Queue Command */
typedef struct _ADMIN_DELETE_IO_SQ_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned short QID;
			unsigned short reserved0;
		};
	};
} ADMIN_DELETE_IO_SQ_DW10;

/* Identify Command */
typedef struct _ADMIN_IDENTIFY_COMMAND_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned int CNS			:1;
			unsigned int reserved0		:31;
		};
	};
} ADMIN_IDENTIFY_COMMAND_DW10;

/* Get Log Page Command */
typedef struct _ADMIN_GET_LOG_PAGE_DW10
{
	union {
		unsigned int dword;
		struct {
			unsigned char LID;
			unsigned char reserved0;
			unsigned short NUMD			:12;
			unsigned short reserved1	:4;
		};
	};
} ADMIN_GET_LOG_PAGE_DW10;

/* Identify - Power State Descriptor Data Structure */
typedef struct _ADMIN_IDENTIFY_POWER_STATE_DESCRIPTOR
{
	union {
		unsigned int dword[8];
		struct {
			unsigned short MP;
			unsigned short reserved0			:8;
			unsigned short MPS					:1;
			unsigned short NOPS					:1;
			unsigned short reserved1			:6;
			unsigned int ENLAT;
			unsigned int EXLAT;
			unsigned char RRT					:5;
			unsigned char reserved2				:3;
			unsigned char RRL					:5;
			unsigned char reserved3				:3;
			unsigned char RWT					:5;
			unsigned char reserved4				:3;
			unsigned char RWL					:5;
			unsigned char reserved5				:3;
			unsigned int reserved6[4];
		};
	};
} ADMIN_IDENTIFY_POWER_STATE_DESCRIPTOR;

/* Identify Controller Data Structure */
typedef struct _ADMIN_IDENTIFY_CONTROLLER
{
	unsigned short VID;
	unsigned short SSVID;
	unsigned char SN[20];
	unsigned char MN[40];
	unsigned char FR[8];
	unsigned char RAB;
	unsigned char IEEE[3];
	unsigned char CMIC;
	unsigned char MDTS;
	unsigned short CNTLID;
	unsigned char reserved0[176];

	struct
	{
		unsigned short supportsSecuritySendSecurityReceive		:1;
		unsigned short supportsFormatNVM						:1;
		unsigned short supportsFirmwareActivateFirmwareDownload	:1;
		unsigned short reserved0								:13;
	} OACS;

	unsigned char ACL;
	unsigned char AERL;

	struct
	{
		unsigned char firstFirmwareSlotReadOnly					:1;
		unsigned char supportedNumberOfFirmwareSlots			:3;
		unsigned char reserved0									:4;
	} FRMW;

	struct
	{
		unsigned char supportsSMARTHealthInformationLogPage		:1;
		unsigned char suppottsCommandEffectsLogPage				:1;
		unsigned char reserved0									:6;
	} LPA;

	unsigned char ELPE;
	unsigned char NPSS;

	unsigned char AVSCC											:1;
	unsigned char reserved1										:7;

	unsigned char APSTA											:1;
	unsigned char reserved2										:7;

	unsigned char reserved3[246];

	struct
	{
		unsigned char requiredSubmissionQueueEntrySize			:4;
		unsigned char maximumSubmissionQueueEntrySize			:4;
	} SQES;

	struct
	{
		unsigned char requiredCompletionQueueEntrySize			:4;
		unsigned char maximumCompletionQueueEntrySize			:4;
	} CQES;

	unsigned char reserved4[2];
	unsigned int NN;

	struct
	{
		unsigned short supportsCompare							:1;
		unsigned short supportsWriteUncorrectable				:1;
		unsigned short supportsDataSetManagement				:1;
		unsigned short reserved0								:13;
	} ONCS;

	struct
	{
		unsigned short supportsCompareWrite						:1;
		unsigned short reserved0								:15;
	} FUSES;

	struct
	{
		unsigned char formatAppliesToAllNamespaces				:1;
		unsigned char secureEraseAppliesToAllNamespaces			:1;
		unsigned char supportsCryptographicErase				:1;
		unsigned char reserved0									:5;
	} FNA;

	struct
	{
		unsigned char present									:1;
		unsigned char reserved0									:7;
	} VWC;

	unsigned short AWUN;
	unsigned short AWUPF;

	unsigned char NVSCC											:1;
	unsigned char reserved5										:7;

	unsigned char reserved6;

	unsigned short ACWU;

	unsigned char reserved7[2];

	struct
	{
		unsigned int supportsSGL								:1;
		unsigned int reserved0									:15;
		unsigned int supportsSGLBitBucketDescriptor				:1;
		unsigned int reserved1									:15;
	} SGLS;


	unsigned char reserved8[164];
	unsigned char reserved9[1344];

	ADMIN_IDENTIFY_POWER_STATE_DESCRIPTOR PSDx[32];

	unsigned char VS[1024];

} ADMIN_IDENTIFY_CONTROLLER;




/* Identify - LBA Format Data Structure */
typedef struct _ADMIN_IDENTIFY_FORMAT_DATA
{
	unsigned short MS;
	unsigned char LBADS;
	unsigned char RP				:2;
	unsigned char reserved			:6;
} ADMIN_IDENTIFY_FORMAT_DATA;

/* Identify Namespace Data Structure */
typedef struct _ADMIN_IDENTIFY_NAMESPACE
{
	unsigned int NSZE[2];
	unsigned int NCAP[2];
	unsigned int NUSE[2];

	struct
	{
		unsigned char supportsThinProvisioning				:1;
		unsigned char reserved0								:7;
	} NSFEAT;

	unsigned char NLBAF;

	struct
	{
		unsigned char supportedCombination					:4;
		unsigned char supportsMetadataAtEndOfLBA			:1;
		unsigned char reserved0								:3;
	} FLBAS;

	struct
	{
		unsigned char supportsMetadataAsPartOfLBA			:1;
		unsigned char supportsMetadataAsSeperate			:1;
		unsigned char reserved0								:6;
	} MC;


	struct
	{
		unsigned char supportsProtectionType1				:1;
		unsigned char supportsProtectionType2				:1;
		unsigned char supportsProtectionType3				:1;
		unsigned char supportsProtectionFirst8				:1;
		unsigned char supportsProtectionLast8				:1;
		unsigned char reserved0								:3;
	} DPC;

	struct
	{
		unsigned char protectionEnabled						:3;
		unsigned char protectionInFirst8					:1;
		unsigned char reserved0								:4;
	} DPS;

	struct
	{
		unsigned char supportsMultipathIOSharing			:1;
		unsigned char reserved0								:7;
	} NMIC;

	struct
	{
		unsigned char supportsPersistThroughPowerLoss		:1;
		unsigned char supportsWriteExclusiveReservation		:1;
		unsigned char supportsWriteExclusiveRegistrants		:1;
		unsigned char supportsExclusiveAccessRegistrants	:1;
		unsigned char supportsWriteExclusiveAllRegistrants	:1;
		unsigned char supportsExclusiveAccessAllRegistrants	:1;
		unsigned char reserved0								:2;
	} RESCAP;

	unsigned char reserved0[88];
	unsigned char EUI64[8];

	ADMIN_IDENTIFY_FORMAT_DATA LBAFx[16];

	unsigned char reserved1[192];
	unsigned char VS[3712];

} ADMIN_IDENTIFY_NAMESPACE;


/* IO Write Command */
typedef struct _IO_WRITE_COMMAND_DW12
{
	union {
		unsigned int dword;
		struct {
			unsigned short NLB;
			unsigned short reserved0				:10;
			unsigned short PRINFO					:4;
			unsigned short FUA						:1;
			unsigned short LR						:1;
		};
	};
} IO_WRITE_COMMAND_DW12;

typedef struct _IO_WRITE_COMMAND_DW13
{
	union {
		unsigned int dword;
		struct {
			struct
			{
				unsigned char AccessFrequency			:4;
				unsigned char AccessLatency				:2;
				unsigned char SequentialRequest			:1;
				unsigned char Incompressible			:1;
			} DSM;
			unsigned char reserved0[3];
		};
	};
} IO_WRITE_COMMAND_DW13;

typedef struct _IO_WRITE_COMMAND_DW15
{
	union {
		unsigned int dword;
		struct {
			unsigned short ELBAT;
			unsigned short ELBATM;
		};
	};
} IO_WRITE_COMMAND_DW15;


/* IO Read Command */
typedef struct _IO_READ_COMMAND_DW12
{
	union {
		unsigned int dword;
		struct {
			unsigned short NLB;
			unsigned short reserved0				:10;
			unsigned short PRINFO					:4;
			unsigned short FUA						:1;
			unsigned short LR						:1;
		};
	};
} IO_READ_COMMAND_DW12;

typedef struct _IO_READ_COMMAND_DW13
{
	union {
		unsigned int dword;
		struct {
			struct
			{
				unsigned char AccessFrequency			:4;
				unsigned char AccessLatency				:2;
				unsigned char SequentialRequest			:1;
				unsigned char Incompressible			:1;
			} DSM;
			unsigned char reserved0[3];
		};
	};
} IO_READ_COMMAND_DW13;

typedef struct _IO_READ_COMMAND_DW15
{
	union {
		unsigned int dword;
		struct {
			unsigned short ELBAT;
			unsigned short ELBATM;
		};
	};
} IO_READ_COMMAND_DW15;


/* IO Dataset Management Command */
typedef struct _IO_DATASET_MANAGEMENT_COMMAND_DW10
{
	unsigned int NR							:8;
	unsigned int reserved0					:24;
} _IO_DATASET_MANAGEMENT_COMMAND_DW10;

typedef struct _IO_DATASET_MANAGEMENT_COMMAND_DW11
{
	unsigned int IDR						:1;
	unsigned int IDW						:1;
	unsigned int AD							:1;
	unsigned int reserved0					:29;
} _IO_DATASET_MANAGEMENT_COMMAND_DW11;

typedef struct _DATASET_MANAGEMENT_CONTEXT_ATTRIBUTES
{
	unsigned int AF							:4;
	unsigned int AL							:2;
	unsigned int reserved0					:2;
	unsigned int SR							:1;
	unsigned int SW							:1;
	unsigned int WP							:1;
	unsigned int reserved1					:13;
	unsigned int CommandAccessSize			:8;
} DATASET_MANAGEMENT_CONTEXT_ATTRIBUTES;

typedef struct _DATASET_MANAGEMENT_RANGE
{
    DATASET_MANAGEMENT_CONTEXT_ATTRIBUTES ContextAttributes;
	unsigned int lengthInLogicalBlocks;
	unsigned int startingLBA[2];
} DATASET_MANAGEMENT_RANGE;

#pragma pack(pop)


typedef struct _NVME_ADMIN_QUEUE_STATUS
{
	unsigned char enable;
	unsigned char sqValid;
	unsigned char cqValid;
	unsigned char irqEn;
} NVME_ADMIN_QUEUE_STATUS;

typedef struct _NVME_IO_SQ_STATUS
{
	unsigned char valid;
	unsigned char cqVector;
	unsigned short qSzie;
	unsigned int pcieBaseAddrL;
	unsigned int pcieBaseAddrH;
} NVME_IO_SQ_STATUS;

typedef struct _NVME_IO_CQ_STATUS
{
	unsigned char valid;
	unsigned char irqEn;
	unsigned short reserved0;
	unsigned short irqVector;
	unsigned short qSzie;
	unsigned int pcieBaseAddrL;
	unsigned int pcieBaseAddrH;
} NVME_IO_CQ_STATUS;

typedef struct _NVME_STATUS
{
	unsigned int status;
	unsigned int cacheEn;
	NVME_ADMIN_QUEUE_STATUS adminQueueInfo;
	NVME_IO_SQ_STATUS ioSqInfo[MAX_NUM_OF_IO_SQ];
	NVME_IO_CQ_STATUS ioCqInfo[MAX_NUM_OF_IO_CQ];
} NVME_CONTEXT;



#endif	//__NVME_H_
