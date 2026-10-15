import numpy as np
import sys
import os
import glob
import h5py
import tables
import random
from itertools import groupby
import uproot


def main(fout,fins):
  fout = tables.open_file(fout, mode='a')
  root = fout.root
  
  if not type(fins) is list: fins = [fins]
  random.shuffle(fins)

  files = 0
  total_events = 0
  
  for fin in fins:
    try:
        df = uproot.open(fin)['pixelmap']
    except uproot.exceptions.KeyInFileError:
        continue

    root = fout.root
    
    events = np.unique(df['unique_id'])
    
    last_png = -1
    png_tags = np.array(df['prong_tag'])
    png_start_indices = []
    
    evt_ids = np.array(df['unique_id'])
    evt_start_indices = [0]
    
    evt_image_indices = np.where(np.array(df['prong_tag'])==-1)[0]
    print(evt_image_indices)
    cvnmap_index = np.column_stack((evt_ids[evt_image_indices] - 1 + total_events, np.array(df['iplane'])[evt_image_indices], np.array(df['rel_wire'])[evt_image_indices], np.array(df['rel_tick'])[evt_image_indices]))
    
    png_image_indices = np.where(np.array(df['prong_tag'])!=-1)[0]
    


    for ipix, png_tag in enumerate(png_tags):
        if png_tag == -1:
            last_shw = -1
            if ipix != 0:
                if evt_ids[ipix] != evt_ids[ipix - 1]:
                    evt_start_indices.append(ipix)
                if (png_tags[ipix - 1] == -1 or png_tags[ipix - 1] == 1000) and \
                    evt_ids[ipix] != evt_ids[ipix - 1]:
                    png_start_indices.append(ipix)
                
        if png_tag > -1:
            if png_tag >= 1000:
                continue
                png_tags[ipix] = png_tag - 1000
                last_shw = png_tag - 1000
            else:
                png_tags[ipix] = png_tag + last_shw + 1
                #last_png = png_tag
        
            if ipix != 0:
                if png_tags[ipix] != png_tags[ipix - 1]:
                    png_start_indices.append(ipix)

                    
    evt_start_indices = np.array(evt_start_indices)            
    png_start_indices = np.array(png_start_indices)
    
    if len(events) != len(evt_start_indices):
        print("len(events) = "+str(len(events))+", len(evt_start_indices) = "+  str(len(evt_start_indices)))
        continue
    
    png_start_tags = png_tags[png_start_indices]
    png_pdg = np.array(df['prong_true_pdg'])[png_start_indices]
    png_pdg_mom = np.array(df['prong_true_pdg_mom'])[png_start_indices]
    png_tags_orig = np.array(df['prong_tag'])[png_start_indices]
    png_eng = np.array(df['prong_eng'])[png_start_indices]
    png_len = np.array(df['prong_length'])[png_start_indices]
    png_startx = np.array(df['prong_startx'])[png_start_indices]
    png_starty = np.array(df['prong_starty'])[png_start_indices]
    png_startz = np.array(df['prong_startz'])[png_start_indices]
    png_px = np.array(df['prong_px'])[png_start_indices]
    png_py = np.array(df['prong_py'])[png_start_indices]
    png_pz = np.array(df['prong_pz'])[png_start_indices]
    png_true_eng = np.array(df['prong_true_eng'])[png_start_indices]
    png_true_px = np.array(df['prong_true_px'])[png_start_indices]
    png_true_py = np.array(df['prong_true_py'])[png_start_indices]
    png_true_pz = np.array(df['prong_true_pz'])[png_start_indices]
    pdg_labels = {321: 7, 211: 4, 13:1, 11: 0, 22: 6, 2212: 2, 1000010020: 7, 1000020040: 7, 1000130270: 7, 1000010030: 7, 3112: 7}
    png_pad_mask = []
    png_label = []
    png_label_split_photons = []
    png_mother = []
    png_ShwTrk = []
    png_E = []
    png_P = []
    png_input = []
    evt_png_pad_mask = np.zeros(20, dtype=np.int8)
    evt_png_label = -np.ones(20, dtype=np.int8)
    evt_png_label_split_photons = -np.ones(20, dtype=np.int8)
    evt_png_mother = -np.ones(20, dtype=np.int16)
    evt_png_ShwTrk = -np.ones(20, dtype=np.int8)
    evt_png_E = -np.ones(20, dtype=np.float32)
    evt_png_P = -np.ones((3,20), dtype=np.float32)
    evt_png_input = np.zeros((8,20), dtype=np.float32)
    
    for ipng,png in enumerate(png_start_tags):
        if png >= 20: continue
        
        if ipng!=0:
            if png == -1 and png_start_tags[ipng - 1] == -1:
                png_pad_mask.append(evt_png_pad_mask)
                png_label.append(evt_png_label)
                png_label_split_photons.append(evt_png_label_split_photons)
                png_mother.append(evt_png_mother)
                png_ShwTrk.append(evt_png_ShwTrk)
                png_E.append(evt_png_E)
                png_P.append(evt_png_P)
                png_input.append(evt_png_input)
                continue
                
            if png == -1:
                png_pad_mask.append(evt_png_pad_mask)
                png_label.append(evt_png_label)
                png_label_split_photons.append(evt_png_label_split_photons)
                png_mother.append(evt_png_mother)
                png_ShwTrk.append(evt_png_ShwTrk)
                png_E.append(evt_png_E)
                png_P.append(evt_png_P)
                png_input.append(evt_png_input)
                evt_png_pad_mask = np.zeros(20, dtype=np.int8)
                evt_png_label = -np.ones(20, dtype=np.int8)
                evt_png_label_split_photons = -np.ones(20, dtype=np.int8)
                evt_png_mother = -np.ones(20, dtype=np.int16)
                evt_png_ShwTrk = -np.ones(20, dtype=np.int8)
                evt_png_E = -np.ones(20, dtype=np.float32)
                evt_png_P = -np.ones((3,20), dtype=np.float32)
                evt_png_input = np.zeros((8,20), dtype=np.float32)
                png_pad_mask.append(evt_png_pad_mask)
                png_label.append(evt_png_label)
                png_label_split_photons.append(evt_png_label_split_photons)
                png_mother.append(evt_png_mother)
                png_ShwTrk.append(evt_png_ShwTrk)
                png_E.append(evt_png_E)
                png_P.append(evt_png_P)
                png_input.append(evt_png_input)
                continue
            
            if png <= png_start_tags[ipng - 1]:
                png_pad_mask.append(evt_png_pad_mask)
                png_label.append(evt_png_label)
                png_label_split_photons.append(evt_png_label_split_photons)
                png_mother.append(evt_png_mother)
                png_ShwTrk.append(evt_png_ShwTrk)
                png_E.append(evt_png_E)
                png_P.append(evt_png_P)
                png_input.append(evt_png_input)
                evt_png_pad_mask = np.zeros(20, dtype=np.int8)
                evt_png_label = -np.ones(20, dtype=np.int8)
                evt_png_label_split_photons = -np.ones(20, dtype=np.int8)
                evt_png_mother = -np.ones(20, dtype=np.int16)
                evt_png_ShwTrk = -np.ones(20, dtype=np.int8)
                evt_png_E = -np.ones(20, dtype=np.float32)
                evt_png_P = -np.ones((3,20), dtype=np.float32)
                evt_png_input = np.zeros((8,20), dtype=np.float32)
        evt_png_pad_mask=np.array(evt_png_pad_mask)
        evt_png_label = np.array(evt_png_label)
        evt_png_label_split_photons = np.array(evt_png_label_split_photons)
        evt_png_mother = np.array(evt_png_mother)
        evt_png_ShwTrk = np.array(evt_png_ShwTrk)
        evt_png_E = np.array(evt_png_E)
        evt_png_P = np.array(evt_png_P)
        
        evt_png_pad_mask[png] = 1
        try:
            evt_png_label[png] = pdg_labels.get(np.abs(png_pdg[ipng]))
        except TypeError:
            evt_png_label[png] = 7
            print("Unknown pdg: %i" % png_pdg[ipng])
            
        evt_png_mother[png] = png_pdg_mom[ipng]
        evt_png_E[png] = png_true_eng[ipng]
        evt_png_P[:,png] = np.array([png_true_px[ipng], png_true_py[ipng], png_true_pz[ipng]])
        
        this_png_len = png_len[ipng]
        if this_png_len == -1:
            this_png_len = 0
            
        evt_png_input[:,png] = np.array([png_eng[ipng], this_png_len, png_startx[ipng], png_starty[ipng], png_startz[ipng], png_px[ipng], png_py[ipng], png_pz[ipng]])
        
        if png_pdg[ipng] == 22 and png_pdg_mom[ipng] == 111:
            evt_png_label_split_photons[png] = 5
        elif png_pdg[ipng] == 22 and png_pdg_mom[ipng] == 2112:
            evt_png_label_split_photons[png] = 3
        else:
            evt_png_label_split_photons[png] = evt_png_label[png]
            
        if png_tags_orig[ipng] >= 1000:
            evt_png_ShwTrk[png] = 0
        elif png_tags_orig[ipng] >= 0:
            evt_png_ShwTrk[png] = 1
            
    png_pad_mask.append(evt_png_pad_mask)
    png_label.append(evt_png_label)
    png_label_split_photons.append(evt_png_label_split_photons)
    png_mother.append(evt_png_mother)
    png_ShwTrk.append(evt_png_ShwTrk)
    png_E.append(evt_png_E)
    png_P.append(evt_png_P)
    png_input.append(evt_png_input)
    
    
    if len(png_E) != len(evt_start_indices):
        print("len(png_E) = "+str(len(png_E))+", len(evt_start_indices) = "+str(len(evt_start_indices)))
        #continue
        
    if fin == fins[0]:
        fout.create_earray(root, "cvnmap_index", obj=cvnmap_index)
        fout.create_earray(root, "cvnmap_value", obj=np.array(df['wire_charge'])[evt_image_indices])
        fout.create_earray(root, "input_png3d_pad_mask", obj=np.array(png_pad_mask))
        fout.create_earray(root, "mc.png_label", obj=np.array(png_label))
        fout.create_earray(root, "mc.png_label_split_photons", obj=np.array(png_label_split_photons))
        fout.create_earray(root, "mc.png_mother", obj=np.array(png_mother))
        fout.create_earray(root, "png_ShwTrk", obj=np.array(png_ShwTrk))
        fout.create_earray(root, "png_trueE", obj=np.array(png_E))
        fout.create_earray(root, "png_trueP", obj=np.array(png_P))
        fout.create_earray(root, "input_png3d", obj=np.array(png_input))
    else:
        getattr(root, "cvnmap_index").append(cvnmap_index)
        getattr(root, "cvnmap_value").append(np.array(df['wire_charge'])[evt_image_indices])
        getattr(root, "input_png3d_pad_mask").append(png_pad_mask)
        getattr(root, "mc.png_label").append(png_label)
        getattr(root, "mc.png_label_split_photons").append(png_label_split_photons)
        getattr(root, "mc.png_mother").append(png_mother)
        getattr(root, "png_ShwTrk").append(png_ShwTrk)
        getattr(root, "png_trueE").append(png_E)
        getattr(root, "png_trueP").append(png_P)
        getattr(root, "input_png3d").append(png_input)
    
    
    png_tags = png_tags[png_image_indices]
    
    
    
    extra_pngs = np.where(png_tags >= 20)[0]
    png_tags = np.delete(png_tags, extra_pngs)
    png_image_indices = np.delete(png_image_indices, extra_pngs)
    png_cvnmap_index = np.column_stack((np.array(df['unique_id'])[png_image_indices] - 1 + total_events, png_tags, np.array(df['iplane'])[png_image_indices], np.array(df['rel_wire'])[png_image_indices], np.array(df['rel_tick'])[png_image_indices]))
    
    if fin == fins[0]:
        fout.create_earray(root, "png_cvnmap_index", obj=png_cvnmap_index)
        fout.create_earray(root, "png_cvnmap_value", obj=np.array(df['wire_charge'])[png_image_indices])
    else:
        getattr(root, "png_cvnmap_index").append(png_cvnmap_index)
        getattr(root, "png_cvnmap_value").append(np.array(df['wire_charge'])[png_image_indices])
    
    
    
    
    ccnc = np.array(np.array(df['trueCCNC'])[evt_start_indices])
    pdg = np.array(np.array(df['trueNuPDG'])[evt_start_indices])
    #pdg = np.abs(pdg)
    mode  = np.array(np.array(df['trueMode'])[evt_start_indices])
    
    inter = np.where(ccnc == 1, 13, pdg)
    inter = np.where(inter==14, 0, inter)
    inter = np.where(inter==-14, 1, inter)
    inter = np.where(inter==12, 4, inter)
    inter = np.where(inter==-12, 5, inter)
    inter = np.where(inter==16, 8, inter)
    inter = np.where(inter==-16, 9, inter)

    
    if 'nue' in fin:
        isflux=np.ones(len(events),dtype=np.int8)
    else:
        isflux=np.zeros(len(events),dtype=np.int8)
    
    cvn_vars = ['cvnnue','cvnnumu','cvnnutau','cvnnc','cvn0protons','cvn1protons','cvn2protons','cvnNprotons','cvn0pions','cvn1pions','cvn2pions','cvnNpions','cvn0pizeros','cvn1pizeros','cvn2pizeros','cvnNpizeros','cvn0neutrons','cvn1neutrons','cvn2neutrons','cvnNneutrons']
    if fin == fins[0]:
        fout.create_earray(root, "mc.inter", obj=inter)
        fout.create_earray(root, "tpc_id", obj=np.array(df['tpc_id'])[evt_start_indices])
        fout.create_earray(root, "trueE", obj=np.array(df['trueEnergy'])[evt_start_indices])
        fout.create_earray(root, "trueVertex", obj=np.column_stack((np.array(df['trueVertex_X'])[evt_start_indices],np.array(df['trueVertex_Y'])[evt_start_indices], np.array(df['trueVertex_Z'])[evt_start_indices])))
        fout.create_earray(root, "trueP", obj=np.column_stack((np.array(df['truePx'])[evt_start_indices], np.array(df['truePy'])[evt_start_indices], np.array(df['truePz'])[evt_start_indices])))
        fout.create_earray(root, "input_slice", obj=np.column_stack((np.array(df['recoNu_E'])[evt_start_indices], np.array(df['recoVertex_X'])[evt_start_indices], np.array(df['recoVertex_Y'])[evt_start_indices], np.array(df['recoVertex_Z'])[evt_start_indices])))
        fout.create_earray(root, "isNonFlux", obj=isflux)
        for cvn_var in cvn_vars:
            fout.create_earray(root, cvn_var, obj=np.array(df[cvn_var])[evt_start_indices])
            
    else:
        getattr(root, "mc.inter").append(inter)
        getattr(root, "tpc_id").append(np.array(df['tpc_id'])[evt_start_indices])
        getattr(root, "trueE").append(np.array(df['trueEnergy'])[evt_start_indices])
        getattr(root, "trueVertex").append(np.column_stack((np.array(df['trueVertex_X'])[evt_start_indices],np.array(df['trueVertex_Y'])[evt_start_indices], np.array(df['trueVertex_Z'])[evt_start_indices])))
        getattr(root, "trueP").append(np.column_stack((np.array(df['truePx'])[evt_start_indices], np.array(df['truePy'])[evt_start_indices], np.array(df['truePz'])[evt_start_indices])))
        getattr(root, "input_slice").append(np.column_stack((np.array(df['recoNu_E'])[evt_start_indices], np.array(df['recoVertex_X'])[evt_start_indices], np.array(df['recoVertex_Y'])[evt_start_indices], np.array(df['recoVertex_Z'])[evt_start_indices])))
        getattr(root, "isNonFlux").append(isflux)
        for cvn_var in cvn_vars:
            getattr(root, cvn_var).append(np.array(df[cvn_var])[evt_start_indices])
    
    total_events+=len(events)
    files+=1
    print(files)
    print(total_events)
    
  fout.create_earray(root, "cvnmap_shape", obj=np.array([total_events, 3, 350, 350]))
  fout.create_earray(root, "png_cvnmap_shape", obj=np.array([total_events, 20, 3, 350, 350]))
  
  fout.close()

if __name__ == '__main__':
    fout = sys.argv[1]
    #if os.path.isfile(fout):
    #    print('{} already exists. Will not overwrite'.format(fout))
    #    sys.exit()
    fin = []
    for arg in sys.argv[2:]:
        these = glob.glob(arg)
        if type(these) is list: fin = fin + these
        else: fin.append(these)
    print(fout, fin)
    try:
        os.remove(fout)
    except FileNotFoundError:
        pass
    main(fout, fin)
