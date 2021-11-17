#ifndef _BIT_WORK_H
#define _BIT_WORK_H

#define set(bit)		(1<<bit)
#define	set_bit(y,bit)	(y|=(1<<bit))
#define	clr_bit(y,bit)	(y&=~(1<<bit))
#define cpl_bit(y,bit) 	(y^=(1<<bit))
#define tst_bit(y,bit) 	(y&(1<<bit))

#endif
