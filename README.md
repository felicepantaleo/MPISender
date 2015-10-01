# MPISender
    export MV2_PATH=/opt/mvapich2/gdr/2.1a/gnu
    export =path/to/GDRCOPY/install
    export MV2_USE_CUDA=1
        $MV2_PATH/bin/mpirun_rsh -export -np 2 hostA hostB \
        $MV2_PATH/libexec/mvapich2/get_local_rank \
        $MV2_PATH/libexec/mvapich2/osu_latency D D
        
        
        
        
- MV2_USE_CUDA
   * Default: 0 (Disabled)
   * To toggle support for communication from NVIDIA GPUs. For enabling, set
     to 1.

- MV2_CUDA_BLOCK_SIZE
   * Default: 262144
   * To tune pipelined internode transfers between NVIDIA GPUs. Higher values 
     may help applications that use larger messages and are bandwidth critical.

- MV2_GPUDIRECT_LIMIT
   * Default: 8192
   * To tune the hybrid design that uses pipelining and GPUDirect RDMA for
     maximum performance while overcoming P2P bandwidth bottlenecks seen on
     modern systems. GPUDirect RDMA is used only for messages with size less
     than or equal to this limit. It has to be tuned based on the node
     architecture, the processor, the GPU and the IB card. 

- MV2_USE_GPUDIRECT_RECEIVE_LIMIT
   * Default: 131072
   * To tune the hybrid design that uses pipelining and GPUDirect RDMA for
     maximum performance while overcoming P2P read bandwidth bottlenecks seen
     on modern systems.  Lower values (16384) may help improve performance on
     nodes with multiple GPUs and IB adapters. It has to be tuned based on the
     node architecture, the processor, the GPU and the IB card. 


GDRCOPY feature: Usage and Tuning Parameters
--------------------------------------------

- MV2_USE_GPUDIRECT_GDRCOPY_LIMIT
   * Default: 8192
   * To tune the local transfer threshold using gdrcopy module between GPU and
     CPU for point to point communications. It has to be tuned based on the
     node architecture, the processor, the GPU and the IB card.

- MV2_USE_GPUDIRECT_GDRCOPY_NAIVE_LIMIT
   * Default: 8192
   * To tune the local transfer threshold using gdrcopy module between GPU and
     CPU for collective communications. It has to be tuned based on the node
     architecture, the processor, the GPU and the IB card.

Loopback Feature: Usage and Tuning Parameters
---------------------------------------------

- MV2_USE_GPUDIRECT_LOOPBACK_LIMIT
   * Default: 8192
   * To tune the transfer threshold using loopback design for point to point
     communications.  It has to be tuned based on the node architecture, the
     processor, the GPU and the IB card.

- MV2_USE_GPUDIRECT_LOOPBACK_NAIVE_LIMIT
   * Default: 8192
   * To tune the transfer threshold using loopback design for collective
     communications.  It has to be tuned based on the node architecture, the
     processor, the GPU and the IB card.
        



git pull; mpic++ pingpong.cpp -fpermissive -I/usr/local/cuda/include