Message from Alejandro in Slack:

For atmnu, I actually only modified the preprocessing module. I have not worked on the ART implementation. That should be in /exp/dune/app/users/ayankele/preprocess_atmnu/, the RecoEnergyS_module.cc somewhere inside of dunereco. The make_text_file_to_root_trks_shws.C then resizes them and puts them in a simpler root output format, which we then convert with root2hdf5.


The main task of the RecoEnergyS is to get all the necessary information from the ART-format root files into a flat root tree “WC”. This is run with larsoft on the DUNE GPVMs or on the grid as I showed in the large text block from Sep 1st.
The make_text_file_to_root_trks_shws.C is what actually creates the pixel maps. It creates the images by rebinning the charge depositions along the time and charge axes according to the specified dimensions. I’m sharing the original version and one I modified to support the atmnu pixel map dimensions. In theory, if the x_coarse and y_coarse variables match what is chosen for the TimeResolution and WireResolution in TransformerCVNMapper.fcl, the pixel maps should be the same, but I don’t know if anyone checked this.
We then use root2hdf5 to convert the root files to hdf5.

This is where the preprocessing ends for RegCVN. For TransformerCVN, there are a couple more reformatting steps at Alex Shmakov’s request.
First, there is this preprocess2.py script, (or preprocess_atmnu.py) I made. Then, there is Alex’s CreateFullySparseDataset.ipynb. This finally creates a single h5 file than can be used to train the network with the training repository. These last steps only involve reformatting and do not change the pixel maps.
