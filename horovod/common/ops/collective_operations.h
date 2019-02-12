//
// Created by Travis Addair on 2018-12-18.
//

#ifndef HOROVOD_COLLECTIVE_OPERATIONS_H
#define HOROVOD_COLLECTIVE_OPERATIONS_H

#include <iostream>

#include "../common.h"
#include "../global_state.h"
#include "communication_context.h"

namespace horovod {
namespace common {

class HorovodOp {
public:
  HorovodOp(CommunicationContext* comm_context, HorovodGlobalState* global_state);

protected:
  CommunicationContext* comm_context_;
  HorovodGlobalState* global_state_;
};

class AllreduceOp : public HorovodOp {
public:
  AllreduceOp(CommunicationContext* comm_context, HorovodGlobalState* global_state);
  virtual ~AllreduceOp()=default;

  virtual void Allreduce(std::vector<TensorTableEntry>& entries, const std::vector<int32_t>& devices);

protected:
  virtual void MemcpyInFusionBuffer(void* buffer_data_at_offset, TensorTableEntry& e,
                                    std::vector<TensorTableEntry>& entries);
  virtual void MemcpyOutFusionBuffer(void* buffer_data_at_offset, TensorTableEntry& e,
                                     std::vector<TensorTableEntry>& entries);
  virtual void StreamSynchronize(std::vector<TensorTableEntry>& entries);
};

class AllgatherOp : public HorovodOp {
public:
  AllgatherOp(CommunicationContext* comm_context, HorovodGlobalState* global_state);
  virtual ~AllgatherOp()=default;

  virtual void Allgather(std::vector<TensorTableEntry>& entries, const std::vector<int64_t>& tensor_sizes);

protected:
  virtual void DoAllgather(std::vector<TensorTableEntry>& entries, int* recvcounts, int* displcmnts,
                           int64_t** entry_component_offsets, int64_t** entry_component_sizes,
                           int64_t total_size, int element_size);
};

class BroadcastOp : public HorovodOp {
public:
  BroadcastOp(CommunicationContext* comm_context, HorovodGlobalState* global_state);
  virtual ~BroadcastOp()=default;

  virtual void Broadcast(std::vector<TensorTableEntry>& entries);
};

class HierarchicalAllgather : public AllgatherOp {
public:
  HierarchicalAllgather(CommunicationContext* comm_context,
                        HorovodGlobalState* global_state);

  virtual void DoAllgather(std::vector<TensorTableEntry>& entries, int* recvcounts, int* displcmnts,
                           int64_t** entry_component_offsets, int64_t** entry_component_sizes,
                           int64_t total_size, int element_size) override;
};

} // namespace common
} // namespace horovod

#endif //HOROVOD_COLLECTIVE_OPERATIONS_H
