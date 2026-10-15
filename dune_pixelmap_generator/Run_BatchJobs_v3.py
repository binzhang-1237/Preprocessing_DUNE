import sys
import os.path
from subprocess import call

def main(args):
	pnfs_path="/pnfs/dune/scratch/users/bjargows/work/macros/"
	filename      = args[1]
	mymacro       = args[2]
	ndiv          = args[3]
	convert_macro = args[4]
	print "Clean PNFS macro directory"
	cmd = "rm -vf "+pnfs_path+"*"
	call(cmd, shell=True)
	
	cmd = "cp -v "+mymacro+" "+pnfs_path
	print "copy my macro", mymacro
	call(cmd, shell=True)
	cmd = "cp -v "+convert_macro+" "+pnfs_path
	print "copy convert macro", convert_macro
	call(cmd, shell=True)

	print "copy file list"
	cmd = "cp -v "+filename+" "+pnfs_path
	call(cmd, shell=True)
	
	pnfs_macro = pnfs_path+mymacro.split("/")[-1]
	pnfs_hdf5  = pnfs_path+convert_macro.split("/")[-1]
	pnfs_list  = pnfs_path+filename

	output_name = "Pixel_Map_low_resolution_nueccvtx_v3.root"
	
	#n_jobs = 1
	#n_jobs = 1975 ## eminus
	#n_jobs = 506 # Nue MCC10
	#n_jobs = 513 # Nue MCC10
	n_jobs = 1010 # MyNuecc

	cmd = "jobsub_submit -N "+str(n_jobs)+" -G dune --expected-lifetime=8h --memory=8GB --disk=100GB --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC file:///dune/app/users/iseong/myjobsub/batch_jobsub5.sh " + pnfs_list + " " + pnfs_macro + " " + output_name + " " + str(ndiv) + " " + pnfs_hdf5
	
	print cmd
	call(cmd, shell=True)
	
if __name__ == "__main__":
	#filename = "list_of_Reco.txt"
	filename_ref = "list_of_Reco2.txt"
	mymacro_ref = "./macros/make_text_file_v2.C"

        if (len(sys.argv)<5):
                print "Need Inputs: [List of files] [macro name] [N Div (default:1)] [HDF5 converting macro]"
		print "Usage: python Run_BatchJobs_v2.py", filename_ref, mymacro_ref, 1, "./macros/do_convert_loop"
                sys.exit(0)
	file_ok = True
	if not (os.path.isfile(sys.argv[1])):
		print "Wrong list file"
		file_ok = False
	if not (os.path.isfile(sys.argv[2])):
		print "Wrong Macro"
		file_ok = False
	if not (os.path.exists(sys.argv[4])):
		print "Need HDF5 Converting Macro"
		file_ok = False

	if not (file_ok): sys.exit(0)
	
        main(sys.argv)

