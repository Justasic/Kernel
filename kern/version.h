#ifndef __KERN_VERSION_H__
#define __KERN_VERSION_H__

#define KERN_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

#define CURRENT_KERN_VERSION KERN_VERSION(1, 0, 0)

#endif
