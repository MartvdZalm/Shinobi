#ifndef GDT_H
#define GDT_H

#include "types.h"

// Define the structure for a segment descriptor
typedef struct SegmentDescriptor
{
	uint16_t limit_lo; 	// Low 16 bits of segment limit
	uint16_t base_lo; 	// Low 16 bits of segment base address
	uint8_t base_hi; 	// Middle 8 bits of segment base address
	uint8_t type;		// Segment type (code, data, etc.)
	uint8_t flags_limit_hi; // Flags and high 4 bits of segment limit
	uint8_t base_vhi;	// High 8 bits of segment base address
}__attribute__((packed)) SegmentDescriptor;

typedef struct GlobalDescriptorTable
{
	SegmentDescriptor nullSegmentSelector;		// Null segment descriptor
	SegmentDescriptor unusedSegmentSelector;	// Unused segment descriptor
	SegmentDescriptor codeSegmentSelector;		// Code segment descriptor
	SegmentDescriptor dataSegmentSelector;		// Data segment descriptor
} GlobalDescriptorTable;

void SegmentDescriptorInit(SegmentDescriptor *descriptor, uint32_t base, uint32_t limit,  uint8_t type);
uint32_t SegmentDescriptorBase(SegmentDescriptor *descriptor);
uint32_t SegmentDescriptorLimit(SegmentDescriptor *descriptor);

void GlobalDescriptorTableInit(GlobalDescriptorTable *gdt);
uint16_t CodeSegmentSelector();
uint16_t DataSegmentSelector();

#endif
