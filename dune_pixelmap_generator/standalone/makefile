CPPFLAGS=`root-config --cflags` -g
LDFLAGS=`root-config --ldflags --glibs` -lCore -lTree

all: do_convert_for_numu do_convert_prong do_convert_loop do_convert_3D do_convert_trks_shws

do_convert_for_numu: ConvertToHDF5_for_numu.cc
	h5c++ $(LDFLAGS) $(CPPFLAGS) -o do_convert_for_numu ConvertToHDF5_for_numu.cc

do_convert_prong: ConvertToHDF5_prong.cc
	h5c++ $(LDFLAGS) $(CPPFLAGS) -o do_convert_prong ConvertToHDF5_prong.cc

do_convert_loop: ConvertToHDF5_loop2.cc
	h5c++ $(LDFLAGS) $(CPPFLAGS) -o do_convert_loop ConvertToHDF5_loop2.cc

do_convert_3D: ConvertToHDF5_3D.cc
	h5c++ $(LDFLAGS) $(CPPFLAGS) -o do_convert_3D ConvertToHDF5_3D.cc

do_convert_trks_shws: ConvertToHDF5_trks_shws.cc
	h5c++ $(LDFLAGS) $(CPPFLAGS) -o do_convert_trks_shws ConvertToHDF5_trks_shws.cc

clean:
	rm do_convert_for_numu do_convert_prong do_convert_loop ConvertToHDF5_loop2.o ConvertToHDF5_prong.o ConvertToHDF5_for_numu.o do_convert_3D ConvertToHDF5_3D.o do_convert_trks_shws ConvertToHDF5_trks_shws.o
