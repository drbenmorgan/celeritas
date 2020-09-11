
if [ x$1 = "xcuda" ]; then export OPT_CUDA=yes; fi


#.. add ninja and clang-format to PATH
export PATH=/home/lima/work/ninja:$PATH
export PATH=/tmp/lima/spack/opt/spack/linux-scientific7-skylake_avx512/gcc-8.3.0/llvm-10.0.0-jnrzwc5he7gdmed6eh4zhktxn4ssavjh/bin:$PATH

module load gnu8/8.3.0

export SPACK_ROOT=/home/lima/work/spack
source $SPACK_ROOT/share/spack/setup-env.sh

#.. use 'module avail' to see what is available for use
#.. use 'module spider' to see what else can be installed
#module load hdf5
module load openmpi3
module load cmake/3.14.3
module load git-2.27.0-gcc-8.3.0-hxn6o2o
module load ninja-1.10.0-gcc-8.3.0-aojpejm
module load git-lfs-2.11.0-gcc-8.3.0-s3lmzcw
module load cuda10/10.2

module load xerces-c-3.2.2-gcc-8.3.0-or75dzn
module load veccore-0.6.0-gcc-8.3.0-yxmkmgo

if [ x$OPT_CUDA = xyes ]
then
    ### CUDA-based
    echo Setting up for CUDA
    module load vecgeom-master-gcc-8.3.0-lm3e6fw
    alias build='cd /home/lima/work/cele/build-cuda && ninja &> x.x; cat x.x; grep -i error x.x | wc'
    alias recmake='source ~/work/cele/scripts/build/lq-cuda.sh'
else
    ### non-CUDA
    echo Setting up for CPU
    module load vecgeom-1.1.7-gcc-8.3.0-ddfsm5l
    alias build='cd /home/lima/work/cele/build && ninja &> x.x; cat x.x; grep -i error x.x | wc'
    alias recmake='source ~/work/cele/scripts/build/lq.sh'
fi

###... old stuff, do not use without modifications!
###alias vostok='cd /home/lima/work/cele/build && rm -rf CMake* *.cmake *.ninja .ninja* lib/ src/ bin/ external/ Testing/ test/ Dart* && cmake -C /home/lima/work/cele/scripts/build/lq.cmake -G Ninja ..'
###spack load vecgeom
