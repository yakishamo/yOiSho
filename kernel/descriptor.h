#ifndef DESCRIPTOR_H
#define DESCRIPTOR_H

typedef enum DescriptorType DescriptorType;

enum DescriptorType {
  kLdt = 0x2,
  kTssAvailable = 0x9,
  kTssBusy = 0xb,
};

#endif /* DESCRIPTOR_H */
