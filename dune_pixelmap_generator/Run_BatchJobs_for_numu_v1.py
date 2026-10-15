import sys
import os.path
from subprocess import call

def main(args):
	pnfs_path="/pnfs/dune/scratch/users/iseong/work/macros_numu/"
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
	output_name = "Pixel_Map_low_resolution_numu.root"
	
	#n_jobs = 1
	#n_jobs = 461 # numu
	n_jobs = 500 # numu
	cmd = "jobsub_submit -N "+str(n_jobs)+" -G dune --expected-lifetime=24h --memory=8GB --disk=100GB --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC file:///dune/app/users/iseong/myjobsub/batch_jobsub_for_numu.sh " + pnfs_list + " " + pnfs_macro + " " + output_name + " " + str(ndiv) + " " + pnfs_hdf5
	
	print cmd
	call(cmd, shell=True)
	
if __name__ == "__main__":
	filename_ref = "list_of_Reco_numu_Jan.txt"
	mymacro_ref = "./macros_numu/make_text_file_to_root_v2_numu_v3.C"

        if (len(sys.argv)<5):
                print "Need Inputs: [List of files] [macro name] [N Div] [HDF5 converting macro]"
		print "Usage: python Run_BatchJobs_for_numu_v1.py", filename_ref, mymacro_ref, 1, "./macros_numu/do_convert_for_numu"
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

