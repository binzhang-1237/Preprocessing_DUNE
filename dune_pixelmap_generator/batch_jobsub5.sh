#!/bin/bash
umask 002
echo "Process Number"
echo $PROCESS
whoami
uname -a
cat /etc/issue
cat /proc/cpuinfo

export DISPLAY=0

source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh 
#setup larsoft v06_64_00 -q e14:prof
#setup larsoft v06_70_00 -q e15:prof
#setup larsoft v07_06_00 -q e17:prof
setup larsoft v08_08_00 -q e17:prof
#setup hdf5 v1_10_1c -q e15
setup hdf5 v1_10_2a -q e17

filelist=$1
mymacro_full=$2
output_prefix=$3
ndiv=$4
convert_macro_full=$5

my_condor_work=${_CONDOR_SCRATCH_DIR}/work

mkdir -p ${my_condor_work}
cd ${my_condor_work}

ifdh cp -D $filelist ./
listname=`basename "$filelist"`

let num=$PROCESS*1+1
echo $listname $num
input=$(tail -n+${num} $listname | head -n1)

#inputfile=$(echo $input | awk -F'/' '{print $NF}') 
inputpath=`dirname "$input"`
inputfile=`basename "$input"`

mymacro=`basename "$mymacro_full"`
convert_macro=`basename "$convert_macro_full"`

search=.$(echo $output_prefix | awk -F'.' '{print $NF}')
replace=_$PROCESS$search
output="${output_prefix/$search/$replace}"
output_h5=${output%.root}.h5
output_log=log_${output%.root}.log
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_electron1"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_electron2"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_electron4"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_electron_new"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_electron_new2"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_electron_new3"

#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nue_high"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nue_low"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nue_low2"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nue_low3"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nue_low3"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nuemcc10_low3"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nuecc_low4"
outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nuecc_low5"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nuecc_vtx"
#outputpath="/pnfs/dune/scratch/users/iseong/work/pixelmap_nuecc_vtx_v4"

echo Input: ${inputpath}/${inputfile}
echo Ouput: ${output}



echo Copy Input file and My Macro
ifdh cp -D ${input} ./
ifdh cp -D ${mymacro_full} ./
ifdh cp -D ${convert_macro_full} ./
chmod 777 ${convert_macro}
ls -lhrt

echo Run my macro
#echo root -l -q -b `printf "${mymacro}(\"$inputfile\",\"$output\",1)" `
#root -l -q -b `printf "${mymacro}(\"$inputfile\",\"$output\",1)" `
echo root -l -q -b `printf "${mymacro}(\"$inputfile\",\"$output\",$ndiv)" ` # make text file with real det. resolution
root -l -q -b `printf "${mymacro}(\"$inputfile\",\"$output\",$ndiv)" ` | tee ${output_log}
echo "Done my macro"
echo "Converting..."
./${convert_macro} ${output} ${output_h5}
echo "Done Converting"
ls -lhrt

ifdh cp -D ${output} ${outputpath} || exit 14
ifdh cp -D ${output_h5} ${outputpath} || exit 15
ifdh cp -D ${output_log} ${outputpath} || exit 16
#if [ -f "${output}" ]; then
#  ifdh cp ./${output} /pnfs/dune/scratch/users/iseong/work/mytest || exit 4
#else
#  echo No output ${output}
#  exit 10
#fi
#cd /pnfs/dune/scratch/users/iseong/work/mytest

cd ${_CONDOR_SCRATCH_DIR}
rm -rf *



