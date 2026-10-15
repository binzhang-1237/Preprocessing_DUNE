import sys
import os
import os.path
from subprocess import call

def main(args):
	pnfs_path="/pnfs/dune/scratch/users/bjargows/work/macros/"
	filename      = args[1]
	mymacro       = args[2]
	type          = args[3]
	convert_macro = args[4]
	print( "Clean PNFS macro directory")
	cmd = "rm -vf "+pnfs_path+"*"
	call(cmd, shell=True)
	
	cmd = "cp -v "+mymacro+" "+pnfs_path
	print( "copy my macro", mymacro)
	call(cmd, shell=True)
	cmd = "cp -v "+convert_macro+" "+pnfs_path
	print( "copy convert macro", convert_macro)
	call(cmd, shell=True)

	print( "copy file list")
	cmd = "cp -v "+filename+" "+pnfs_path
	call(cmd, shell=True)
	
	pnfs_macro = pnfs_path+mymacro.split("/")[-1]
	pnfs_hdf5  = pnfs_path+convert_macro.split("/")[-1]
	pnfs_list  = pnfs_path+filename

	out_dir = "/pnfs/dune/scratch/users/bjargows/work/pixelmap_prong_"+type
	if (os.path.isdir(out_dir)):
		if (len(os.listdir(out_dir))>0):
			print( ("Please move files in {}".format(out_dir)) )
			sys.exit()
	else:
		os.mkdir(out_dir)

	output_name = "Pixel_Map_low_resolution_prong_"+type+".root"
	
	#n_jobs = 1010 # MyNuecc
	with open(pnfs_list, "r") as f:
		n_jobs = len(f.readlines())
	print( "Number of Jobs:", n_jobs )
	cmd = "jobsub_submit -N "+str(n_jobs)+" -G dune --expected-lifetime=8h --memory=3GB --disk=100GB --resource-provides=usage_model=DEDICATED,OPPORTUNISTIC file://./batch_jobsub_prong.sh " + pnfs_list + " " + pnfs_macro + " " + out_dir + " " + output_name + " " + type + " " + pnfs_hdf5
	
	print( cmd )
	call(cmd, shell=True)
	
if __name__ == "__main__":
    main(sys.argv)

