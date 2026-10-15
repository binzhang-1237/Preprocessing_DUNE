#ifdef OLD_HEADER_FILENAME
#include <iostream.h>
#else
#include <iostream>
#endif
#include <string>
#include "H5Cpp.h"
// ROOT stuff
#include "TSystem.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TObjArray.h"
#include "TString.h"


using namespace H5;
const H5std_string  DATASET_NAME( "ArrayOfStructures" );
const int   NX = 5;                    // dataset dimensions
const int   NY = 6;
const int   RANK = 1;

typedef struct data_t{
  int pmidx;
  int ievt, tpc_id, iplane;
  int rel_wire, rel_tick, mean_wire, mean_tick;
  int rebin_wire, rebin_tick;
  int nbins_wire, nbins_tick;
  int shw_id;
  int trueVtxWire, trueVtxTick;
  int trueVtxGlobalWire, trueVtxGlobalTick, pfVtxGlobalWire, pfVtxGlobalTick;
  float shw_tot_energy, wire_charge, wire_corrcharge, hit_tot_energy;
  float trueEnergy, trueVertex_X, trueVertex_Y, trueVertex_Z;
  float truePx, truePy, truePz;
  int trueMode, trueCCNC, trueNuPDG;
  int trueleppdg, nshowers, shwflag;
  float truelepeng, trueleppx, trueleppy, trueleppz;
  float shweng, shwpx, shwpy, shwpz;
  int offsetU, offsetV, offsetZ;
  float recoNu_E, recoVertex_X, recoVertex_Y, recoVertex_Z;
  int trueTPCin, trueDETin;
  int trackCont;
} data_t;

int main(int argc, char** argv){
	if (argc < 3) {
		std::cout << "You need Inputs and Output" << std::endl;
		exit(1);
	}
	std::cout << "Start Converting.. " << std::endl;
	std::cout << "InputFile : " << argv[1] << std::endl;
	std::cout << "OutputFile : " << argv[2] << std::endl;

	//const H5std_string  FILE_NAME( "SDS.h5" );
	const H5std_string  FILE_NAME( argv[2] );
	TChain *ch = new TChain("pixelmap");
	//ch->Add("/pnfs/dune/scratch/users/iseong/work/pixelmap_electron_new/Pixel_Map_low_resolution_1000.root");
	ch->Add(argv[1]);
	
	const int nBranches = ch->GetNbranches();
	TObjArray *branchList = ch->GetListOfBranches();
	std::vector<TString> varname;
	H5std_string *h5_memname = new H5std_string[nBranches];
	data_t dummy_data;

        ch->SetBranchAddress("pmidx", &dummy_data.pmidx);
        ch->SetBranchAddress("ievent", &dummy_data.ievt);
        ch->SetBranchAddress("tpc_id", &dummy_data.tpc_id);
        ch->SetBranchAddress("iplane", &dummy_data.iplane);
        ch->SetBranchAddress("shw_id", &dummy_data.shw_id);
        ch->SetBranchAddress("rel_wire", &dummy_data.rel_wire);
        ch->SetBranchAddress("rel_tick", &dummy_data.rel_tick);
        ch->SetBranchAddress("mean_wire", &dummy_data.mean_wire);
        ch->SetBranchAddress("mean_tick", &dummy_data.mean_tick);
        ch->SetBranchAddress("rebin_wire", &dummy_data.rebin_wire);
        ch->SetBranchAddress("rebin_tick", &dummy_data.rebin_tick);
        ch->SetBranchAddress("nbins_wire", &dummy_data.nbins_wire);
        ch->SetBranchAddress("nbins_tick", &dummy_data.nbins_tick);
        ch->SetBranchAddress("trueVtxWire", &dummy_data.trueVtxWire);
        ch->SetBranchAddress("trueVtxTick", &dummy_data.trueVtxTick);
        ch->SetBranchAddress("shw_tot_energy", &dummy_data.shw_tot_energy);
        ch->SetBranchAddress("wire_charge", &dummy_data.wire_charge);
        ch->SetBranchAddress("wire_corrcharge", &dummy_data.wire_corrcharge);
        ch->SetBranchAddress("hit_tot_energy", &dummy_data.hit_tot_energy);
        ch->SetBranchAddress("trueEnergy", &dummy_data.trueEnergy);
        ch->SetBranchAddress("trueVertex_X", &dummy_data.trueVertex_X);
        ch->SetBranchAddress("trueVertex_Y", &dummy_data.trueVertex_Y);
        ch->SetBranchAddress("trueVertex_Z", &dummy_data.trueVertex_Z);
        ch->SetBranchAddress("truePx", &dummy_data.truePx);
        ch->SetBranchAddress("truePy", &dummy_data.truePy);
        ch->SetBranchAddress("truePz", &dummy_data.truePz);
        ch->SetBranchAddress("trueVtxGlobalWire", &dummy_data.trueVtxGlobalWire);
        ch->SetBranchAddress("trueVtxGlobalTick", &dummy_data.trueVtxGlobalTick);
        ch->SetBranchAddress("pfVtxGlobalWire", &dummy_data.pfVtxGlobalWire);
        ch->SetBranchAddress("pfVtxGlobalTick", &dummy_data.pfVtxGlobalTick);
        ch->SetBranchAddress("trueMode", &dummy_data.trueMode);
        ch->SetBranchAddress("trueCCNC", &dummy_data.trueCCNC);
        ch->SetBranchAddress("trueNuPDG", &dummy_data.trueNuPDG);
	
        ch->SetBranchAddress("trueLepPDG", &dummy_data.trueleppdg);
        ch->SetBranchAddress("trueLepEng", &dummy_data.truelepeng);
        ch->SetBranchAddress("trueLepPx",  &dummy_data.trueleppx);
        ch->SetBranchAddress("trueLepPy",  &dummy_data.trueleppy);
        ch->SetBranchAddress("trueLepPz",  &dummy_data.trueleppz);
        ch->SetBranchAddress("NShw",       &dummy_data.nshowers);
        ch->SetBranchAddress("ShwFlag",    &dummy_data.shwflag);
        ch->SetBranchAddress("ShwEng",     &dummy_data.shweng);
        ch->SetBranchAddress("ShwPx",      &dummy_data.shwpx);
        ch->SetBranchAddress("ShwPy",      &dummy_data.shwpy);
        ch->SetBranchAddress("ShwPz",      &dummy_data.shwpz);
        ch->SetBranchAddress("OffsetU",    &dummy_data.offsetU);
        ch->SetBranchAddress("OffsetV",    &dummy_data.offsetV);
        ch->SetBranchAddress("OffsetZ",    &dummy_data.offsetZ);

        ch->SetBranchAddress("recoNu_E",       &dummy_data.recoNu_E);
        ch->SetBranchAddress("recoVertex_X",   &dummy_data.recoVertex_X);
        ch->SetBranchAddress("recoVertex_Y",   &dummy_data.recoVertex_Y);
        ch->SetBranchAddress("recoVertex_Z",   &dummy_data.recoVertex_Z);

        ch->SetBranchAddress("trueTPCin",      &dummy_data.trueTPCin);
        ch->SetBranchAddress("trueDETin",      &dummy_data.trueDETin);
        ch->SetBranchAddress("trackCont",      &dummy_data.trackCont);

	for (int ib = 0; ib < nBranches; ib++){
		varname.push_back( branchList->At(ib)->GetName() );
		//var[ib] = ch->GetBranch(varname[ib].Data())->GetLeaf(varname[ib].Data());
		h5_memname[ib] = varname[ib].Data();
		std::cout << " ==> " << varname[ib].Data() << std::endl; 
	}

	const int entries = ch->GetEntries();
	//const int entries = 3; 
	std::cout << "Entries: " << entries << std::endl;

	std::cout << "Start to Create HDF5 Files" << std::endl;
	//hsize_t chunk[] = {100000};
	//hsize_t chunk[] = {(hsize_t)entries};
	hsize_t dim[1] = {(hsize_t)entries};
	hsize_t chkdim[1] = {1000000};
	const int subentries = (int)chkdim[0]*10;

	std::cout << "Open HDF5 Files" << std::endl;

	hid_t space    = H5Screate_simple(RANK, dim, NULL);
	hid_t fileout  = H5Fcreate( FILE_NAME.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
	hid_t cparms   = H5Pcreate( H5P_DATASET_CREATE );

	herr_t status;
	std::cout << "Set Chunk and Compression Level" << std::endl;
	status = H5Pset_chunk( cparms, RANK, chkdim );
	status = H5Pset_deflate( cparms, 9); 
	printf ("H5Pset_chunk: %i\n", status);

	std::cout << "Create Compound Type" << std::endl;
       /* Create the memory data type */
	hid_t data_tid = H5Tcreate( H5T_COMPOUND, sizeof(data_t));

 	H5Tinsert(data_tid, h5_memname[0].c_str(), HOFFSET(data_t, pmidx), H5T_NATIVE_INT);
 	H5Tinsert(data_tid, h5_memname[1].c_str(), HOFFSET(data_t, ievt), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[2].c_str(), HOFFSET(data_t, tpc_id), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[3].c_str(), HOFFSET(data_t, iplane), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[4].c_str(), HOFFSET(data_t, shw_id), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[5].c_str(), HOFFSET(data_t, rel_wire), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[6].c_str(), HOFFSET(data_t, rel_tick), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[7].c_str(), HOFFSET(data_t, mean_wire), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[8].c_str(), HOFFSET(data_t, mean_tick), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[9].c_str(), HOFFSET(data_t, rebin_wire), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[10].c_str(), HOFFSET(data_t, rebin_tick), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[11].c_str(), HOFFSET(data_t, nbins_wire), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[12].c_str(), HOFFSET(data_t, nbins_tick), H5T_NATIVE_INT);

	H5Tinsert(data_tid, h5_memname[13].c_str(), HOFFSET(data_t, trueVtxWire), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[14].c_str(), HOFFSET(data_t, trueVtxTick), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[15].c_str(), HOFFSET(data_t, trueVtxGlobalWire), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[16].c_str(), HOFFSET(data_t, trueVtxGlobalTick), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[17].c_str(), HOFFSET(data_t, pfVtxGlobalWire), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[18].c_str(), HOFFSET(data_t, pfVtxGlobalTick), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[19].c_str(), HOFFSET(data_t, shw_tot_energy), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[20].c_str(), HOFFSET(data_t, wire_charge), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[21].c_str(), HOFFSET(data_t, wire_corrcharge), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[22].c_str(), HOFFSET(data_t, hit_tot_energy), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[23].c_str(), HOFFSET(data_t, trueEnergy), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[24].c_str(), HOFFSET(data_t, trueVertex_X), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[25].c_str(), HOFFSET(data_t, trueVertex_Y), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[26].c_str(), HOFFSET(data_t, trueVertex_Z), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[27].c_str(), HOFFSET(data_t, truePx), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[28].c_str(), HOFFSET(data_t, truePy), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[29].c_str(), HOFFSET(data_t, truePz), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[30].c_str(), HOFFSET(data_t, trueMode), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[31].c_str(), HOFFSET(data_t, trueCCNC), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[32].c_str(), HOFFSET(data_t, trueNuPDG), H5T_NATIVE_INT);

	H5Tinsert(data_tid, h5_memname[33].c_str(), HOFFSET(data_t, trueleppdg), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[34].c_str(), HOFFSET(data_t, truelepeng), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[35].c_str(), HOFFSET(data_t, trueleppx), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[36].c_str(), HOFFSET(data_t, trueleppy), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[37].c_str(), HOFFSET(data_t, trueleppz), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[38].c_str(), HOFFSET(data_t, nshowers), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[39].c_str(), HOFFSET(data_t, shwflag), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[40].c_str(), HOFFSET(data_t, shweng), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[41].c_str(), HOFFSET(data_t, shwpx), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[42].c_str(), HOFFSET(data_t, shwpy), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[43].c_str(), HOFFSET(data_t, shwpz), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[44].c_str(), HOFFSET(data_t, offsetU), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[45].c_str(), HOFFSET(data_t, offsetV), H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[46].c_str(), HOFFSET(data_t, offsetZ), H5T_NATIVE_INT);

	H5Tinsert(data_tid, h5_memname[47].c_str(), HOFFSET(data_t, recoNu_E),     H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[48].c_str(), HOFFSET(data_t, recoVertex_X), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[49].c_str(), HOFFSET(data_t, recoVertex_Y), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[50].c_str(), HOFFSET(data_t, recoVertex_Z), H5T_NATIVE_FLOAT);
	H5Tinsert(data_tid, h5_memname[51].c_str(), HOFFSET(data_t, trueTPCin),    H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[52].c_str(), HOFFSET(data_t, trueDETin),    H5T_NATIVE_INT);
	H5Tinsert(data_tid, h5_memname[53].c_str(), HOFFSET(data_t, trackCont),    H5T_NATIVE_INT);

	std::cout << "Craete dataset" << std::endl;
	hid_t dataset = H5Dcreate1( fileout, DATASET_NAME.c_str(), data_tid, space, cparms);

        /* Create memory space for slab writes */
	std::cout << "Crate Memory Space for slap" << std::endl;
	const int nloop = entries/subentries;
	const int remain = entries - nloop*subentries;
	//hsize_t slabsize[1] = {1};
	hsize_t slabsize1[1] = {(hsize_t)subentries};
	hsize_t slabsize2[1] = {(hsize_t)remain};
	//hsize_t maxdim[1] = {H5S_UNLIMITED};
	//hid_t memspace = H5Screate_simple(1, slabsize, maxdim);

	std::cout << "Prepare Loop => SubEntries: " << subentries << " Nloop: " << nloop << " Remain:" << remain << std::endl;
	hsize_t offset[1]={0};
	hsize_t count1[1]={(hsize_t)subentries};
	hsize_t count2[1]={(hsize_t)remain};
	data_t *data1 = new data_t[subentries];
	data_t *data2 = new data_t[remain];

	std::cout << "Start Loop to make dataset " << std::endl;
	unsigned int ie = 0; 
	hid_t memspace;
	memspace = H5Screate_simple(1, slabsize1, NULL);
	for (int iloop = 0; iloop < nloop; iloop++){
		for (int ii = 0; ii < subentries; ii++){
			if (ie%1000000 == 0) printf ("%d : %.1f\n", ie, (double)ie/double(entries)*100.);
			ch->GetEntry(ie);
			data1[ii] = dummy_data;
			ie++;
		}
		std::cout << "Save Data iloop = " << iloop << std::endl;
		space = H5Dget_space(dataset);
		status = H5Sselect_hyperslab( space, H5S_SELECT_SET, offset, NULL, count1, NULL);
		status = H5Dwrite(dataset, data_tid, memspace, space, H5P_DEFAULT, data1);
		offset[0] = offset[0] + subentries;
		status = H5Sclose(space); 
	}

	memspace = H5Screate_simple(1, slabsize2, NULL);
	for (int ii = 0; ii < remain; ii++){
		if (ie%1000000 == 0) printf ("%d : %.1f\n", ie, (double)ie/double(entries)*100.);
		ch->GetEntry(ie);
		data2[ii] = dummy_data;
		ie++;
	}
	printf ("Loop End with %d : %.1f\n", ie, (double)ie/double(entries)*100.);
	std::cout << "Save Data After last loop" << std::endl;
	space = H5Dget_space(dataset);
	status = H5Sselect_hyperslab( space, H5S_SELECT_SET, offset, NULL, count2, NULL);
	status = H5Dwrite(dataset, data_tid, memspace, space, H5P_DEFAULT, data2);
	offset[0] = offset[0] + subentries;
	status = H5Sclose(space); 

	std::cout << "Close HDF5" << std::endl;
	status = H5Pclose(cparms);
	status = H5Tclose(data_tid);
	status = H5Sclose(memspace);
	status = H5Dclose(dataset);
	status = H5Fclose(fileout);


	//plist->setDeflate(9);
	//delete plist;
	
	std::cout << "Done to convert root file into hdf5 file" << std::endl;

	return 0;
}
