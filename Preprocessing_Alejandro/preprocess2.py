import numpy as np
import sys
import os
import glob
import h5py
import tables
import random
from itertools import groupby


def main(fout,fins):
  fout = tables.open_file(fout, mode='a')
  root = fout.root
  
  if not type(fins) is list: fins = [fins]
  random.shuffle(fins)

  files = 0
  total_events = 0
  
  for fin in fins:
    df=h5py.File(fin, 'r')['ArrayOfStructures']
    
    events = np.unique(df['unique_id'])
    
    evt_image_indices = np.where(df['prong_tag']==-1)[0]
    
    event_pixel_indices = np.column_stack((df['unique_id'][evt_image_indices] - 1 + total_events, df['iplane'][evt_image_indices], df['rel_wire'][evt_image_indices], df['rel_tick'][evt_image_indices]))
    event_pixel_values = df['wire_corrcharge'][evt_image_indices]
    
    prong_image_indices = np.where(df['prong_tag']!=-1)[0]
    
    last_prong = -1
    prong_tags = np.array(df['prong_tag'])
    prong_start_indices = []
    
    evt_ids = np.array(df['unique_id'])
    evt_start_indices = [0]

    for ipix, prong_tag in enumerate(prong_tags):
        if prong_tag == -1:
            last_shw = -1
            if ipix != 0:
                if evt_ids[ipix] != evt_ids[ipix - 1]:
                    evt_start_indices.append(ipix)
                if prong_tags[ipix - 1] == -1 and evt_ids[ipix] != evt_ids[ipix - 1]:
                    prong_start_indices.append(ipix)
                
        if prong_tag > -1:
            if prong_tag >= 1000:
                prong_tags[ipix] = prong_tag - 1000
                last_shw = prong_tag - 1000
            else:
                prong_tags[ipix] = prong_tag + last_shw + 1
                #last_prong = prong_tag
        
            if ipix != 0:
                if prong_tags[ipix] != prong_tags[ipix - 1]:
                    prong_start_indices.append(ipix)

                    
    evt_start_indices = np.array(evt_start_indices)            
    prong_start_indices = np.array(prong_start_indices)
    
    if len(events) != len(evt_start_indices):
        fout.close()
        sys.exit(f"Error in {fin}: found {len(events)} events but "
                 f"{len(evt_start_indices)} event start indices")
    
    prong_start_tags = prong_tags[prong_start_indices]
    prong_pdg = df['prong_true_pdg'][prong_start_indices]
    prong_pdg_mom = df['prong_true_pdg_mom'][prong_start_indices]
    prong_tags_orig = df['prong_tag'][prong_start_indices]
    prong_eng = df['prong_eng'][prong_start_indices]
    prong_len = df['prong_length'][prong_start_indices]
    prong_startx = df['prong_startx'][prong_start_indices]
    prong_starty = df['prong_starty'][prong_start_indices]
    prong_startz = df['prong_startz'][prong_start_indices]
    prong_px = df['prong_px'][prong_start_indices]
    prong_py = df['prong_py'][prong_start_indices]
    prong_pz = df['prong_pz'][prong_start_indices]
    prong_true_eng = df['prong_true_eng'][prong_start_indices]
    prong_true_px = df['prong_true_px'][prong_start_indices]
    prong_true_py = df['prong_true_py'][prong_start_indices]
    prong_true_pz = df['prong_true_pz'][prong_start_indices]
    pdg_labels = {321: 7, 211: 4, 13:1, 11: 0, 22: 6, 2212: 2, 1000010020: 7, 1000020040: 7, 1000130270: 7, 1000010030: 7, 3112: 7}
    prong_pad_mask = []
    prong_label = []
    prong_label_split_photons = []
    prong_mother = []
    prong_ShwTrk = []
    prong_E = []
    prong_P = []
    prong_input = []
    evt_prong_pad_mask = np.zeros(20, dtype=np.int8)
    evt_prong_label = -np.ones(20, dtype=np.int8)
    evt_prong_label_split_photons = -np.ones(20, dtype=np.int8)
    evt_prong_mother = -np.ones(20, dtype=np.int32)
    evt_prong_ShwTrk = -np.ones(20, dtype=np.int8)
    evt_prong_E = -np.ones(20, dtype=np.float32)
    evt_prong_P = -np.ones((3,20), dtype=np.float32)
    evt_prong_input = np.zeros((8,20), dtype=np.float32)
    
    for iprong,prong in enumerate(prong_start_tags):
        if prong >= 20: continue
        #print(prong)
        
        if iprong!=0:
            if prong == -1 and prong_start_tags[iprong - 1] == -1:
                prong_pad_mask.append(evt_prong_pad_mask)
                prong_label.append(evt_prong_label)
                prong_label_split_photons.append(evt_prong_label_split_photons)
                prong_mother.append(evt_prong_mother)
                prong_ShwTrk.append(evt_prong_ShwTrk)
                prong_E.append(evt_prong_E)
                prong_P.append(evt_prong_P)
                prong_input.append(evt_prong_input)
                continue
                
            if prong == -1:
                prong_pad_mask.append(evt_prong_pad_mask)
                prong_label.append(evt_prong_label)
                prong_label_split_photons.append(evt_prong_label_split_photons)
                prong_mother.append(evt_prong_mother)
                prong_ShwTrk.append(evt_prong_ShwTrk)
                prong_E.append(evt_prong_E)
                prong_P.append(evt_prong_P)
                prong_input.append(evt_prong_input)
                evt_prong_pad_mask = np.zeros(20, dtype=np.int8)
                evt_prong_label = -np.ones(20, dtype=np.int8)
                evt_prong_label_split_photons = -np.ones(20, dtype=np.int8)
                evt_prong_mother = -np.ones(20, dtype=np.int32)
                evt_prong_ShwTrk = -np.ones(20, dtype=np.int8)
                evt_prong_E = -np.ones(20, dtype=np.float32)
                evt_prong_P = -np.ones((3,20), dtype=np.float32)
                evt_prong_input = np.zeros((8,20), dtype=np.float32)
                prong_pad_mask.append(evt_prong_pad_mask)
                prong_label.append(evt_prong_label)
                prong_label_split_photons.append(evt_prong_label_split_photons)
                prong_mother.append(evt_prong_mother)
                prong_ShwTrk.append(evt_prong_ShwTrk)
                prong_E.append(evt_prong_E)
                prong_P.append(evt_prong_P)
                prong_input.append(evt_prong_input)
                continue
            
            if prong <= prong_start_tags[iprong - 1]:
                prong_pad_mask.append(evt_prong_pad_mask)
                prong_label.append(evt_prong_label)
                prong_label_split_photons.append(evt_prong_label_split_photons)
                prong_mother.append(evt_prong_mother)
                prong_ShwTrk.append(evt_prong_ShwTrk)
                prong_E.append(evt_prong_E)
                prong_P.append(evt_prong_P)
                prong_input.append(evt_prong_input)
                evt_prong_pad_mask = np.zeros(20, dtype=np.int8)
                evt_prong_label = -np.ones(20, dtype=np.int8)
                evt_prong_label_split_photons = -np.ones(20, dtype=np.int8)
                evt_prong_mother = -np.ones(20, dtype=np.int32)
                evt_prong_ShwTrk = -np.ones(20, dtype=np.int8)
                evt_prong_E = -np.ones(20, dtype=np.float32)
                evt_prong_P = -np.ones((3,20), dtype=np.float32)
                evt_prong_input = np.zeros((8,20), dtype=np.float32)
        evt_prong_pad_mask=np.array(evt_prong_pad_mask)
        evt_prong_label = np.array(evt_prong_label)
        evt_prong_label_split_photons = np.array(evt_prong_label_split_photons)
        evt_prong_mother = np.array(evt_prong_mother)
        evt_prong_ShwTrk = np.array(evt_prong_ShwTrk)
        evt_prong_E = np.array(evt_prong_E)
        evt_prong_P = np.array(evt_prong_P)
        
        evt_prong_pad_mask[prong] = 1
        try:
            evt_prong_label[prong] = pdg_labels.get(np.abs(prong_pdg[iprong]))
        except TypeError:
            evt_prong_label[prong] = 7
            print("Unknown pdg: %i" % prong_pdg[iprong])
            
        evt_prong_mother[prong] = prong_pdg_mom[iprong]
        evt_prong_E[prong] = prong_true_eng[iprong]
        evt_prong_P[:,prong] = np.array([prong_true_px[iprong], prong_true_py[iprong], prong_true_pz[iprong]])
        
        this_prong_len = prong_len[iprong]
        if this_prong_len == -1:
            this_prong_len = 0
            
        evt_prong_input[:,prong] = np.array([prong_eng[iprong], this_prong_len, prong_startx[iprong], prong_starty[iprong], prong_startz[iprong], prong_px[iprong], prong_py[iprong], prong_pz[iprong]])
        
        if prong_pdg[iprong] == 22 and prong_pdg_mom[iprong] == 111:
            evt_prong_label_split_photons[prong] = 5
        elif prong_pdg[iprong] == 22 and prong_pdg_mom[iprong] == 2112:
            evt_prong_label_split_photons[prong] = 3
        else:
            evt_prong_label_split_photons[prong] = evt_prong_label[prong]
            
        if prong_tags_orig[iprong] >= 1000:
            evt_prong_ShwTrk[prong] = 0
        elif prong_tags_orig[iprong] >= 0:
            evt_prong_ShwTrk[prong] = 1
            
    prong_pad_mask.append(evt_prong_pad_mask)
    prong_label.append(evt_prong_label)
    prong_label_split_photons.append(evt_prong_label_split_photons)
    prong_mother.append(evt_prong_mother)
    prong_ShwTrk.append(evt_prong_ShwTrk)
    prong_E.append(evt_prong_E)
    prong_P.append(evt_prong_P)
    prong_input.append(evt_prong_input)
    
    
    if len(prong_E) != len(evt_start_indices):
        continue
        
    if fin == fins[0]:
        fout.create_earray(root, "event_pixel_indices", obj=event_pixel_indices)
        fout.create_earray(root, "event_pixel_values", obj=event_pixel_values)
        fout.create_earray(root, "input_prong3d_pad_mask", obj=np.array(prong_pad_mask))
        fout.create_earray(root, "mc.prong_label", obj=np.array(prong_label))
        fout.create_earray(root, "mc.prong_label_split_photons", obj=np.array(prong_label_split_photons))
        fout.create_earray(root, "mc.prong_mother", obj=np.array(prong_mother))
        fout.create_earray(root, "prong_ShwTrk", obj=np.array(prong_ShwTrk))
        fout.create_earray(root, "prong_trueE", obj=np.array(prong_E))
        fout.create_earray(root, "prong_trueP", obj=np.array(prong_P))
        fout.create_earray(root, "input_prong3d", obj=np.array(prong_input))
    else:
        getattr(root, "event_pixel_indices").append(event_pixel_indices)
        getattr(root, "event_pixel_values").append(event_pixel_values)
        getattr(root, "input_prong3d_pad_mask").append(prong_pad_mask)
        getattr(root, "mc.prong_label").append(prong_label)
        getattr(root, "mc.prong_label_split_photons").append(prong_label_split_photons)
        getattr(root, "mc.prong_mother").append(prong_mother)
        getattr(root, "prong_ShwTrk").append(prong_ShwTrk)
        getattr(root, "prong_trueE").append(prong_E)
        getattr(root, "prong_trueP").append(prong_P)
        getattr(root, "input_prong3d").append(prong_input)
    
    
    prong_tags = prong_tags[prong_image_indices]
    
    
    
    extra_prongs = np.where(prong_tags >= 20)[0]
    prong_tags = np.delete(prong_tags, extra_prongs)
    prong_image_indices = np.delete(prong_image_indices, extra_prongs)
    prong_pixel_indices = np.column_stack((df['unique_id'][prong_image_indices] - 1 + total_events, prong_tags, df['iplane'][prong_image_indices], df['rel_wire'][prong_image_indices], df['rel_tick'][prong_image_indices]))
    prong_pixel_values = df['wire_corrcharge'][prong_image_indices]
    
    if fin == fins[0]:
        fout.create_earray(root, "prong_pixel_indices", obj=prong_pixel_indices)
        fout.create_earray(root, "prong_pixel_values", obj=prong_pixel_values)
    else:
        getattr(root, "prong_pixel_indices").append(prong_pixel_indices)
        getattr(root, "prong_pixel_values").append(prong_pixel_values)
    
    
    
    
    ccnc = np.array(df['trueCCNC'][evt_start_indices])
    pdg = np.array(df['trueNuPDG'][evt_start_indices])
    pdg = np.abs(pdg)
    mode  = np.array(df['trueMode'][evt_start_indices])
    
    inter = np.where(ccnc == 1, 13, pdg)
    inter = np.where(np.logical_and(inter==14, mode==0), 0, inter)
    inter = np.where(np.logical_and(inter==14, mode==1), 1, inter)
    inter = np.where(np.logical_and(inter==14, mode==2), 2, inter)
    inter = np.where(inter==14, 3, inter)
    inter = np.where(np.logical_and(inter==12, mode==0), 4, inter)
    inter = np.where(np.logical_and(inter==12, mode==1), 5, inter)
    inter = np.where(np.logical_and(inter==12, mode==2), 6, inter)
    inter = np.where(inter==12, 7, inter)
    
    if 'nue' in fin:
        isflux=np.ones(len(events),dtype=np.int8)
    else:
        isflux=np.zeros(len(events),dtype=np.int8)
    
    cvn_vars = ['cvnnue','cvnnumu','cvnnutau','cvnnc','cvn0protons','cvn1protons','cvn2protons','cvnNprotons','cvn0pions','cvn1pions','cvn2pions','cvnNpions','cvn0pizeros','cvn1pizeros','cvn2pizeros','cvnNpizeros','cvn0neutrons','cvn1neutrons','cvn2neutrons','cvnNneutrons']
    if fin == fins[0]:
        fout.create_earray(root, "mc.inter", obj=inter)
        fout.create_earray(root, "tpc_id", obj=df['tpc_id'][evt_start_indices])
        fout.create_earray(root, "trueE", obj=df['trueEnergy'][evt_start_indices])
        fout.create_earray(root, "trueVertex", obj=np.column_stack((df['trueVertex_X'][evt_start_indices],df['trueVertex_Y'][evt_start_indices], df['trueVertex_Z'][evt_start_indices])))
        fout.create_earray(root, "trueP", obj=np.column_stack((df['truePx'][evt_start_indices], df['truePy'][evt_start_indices], df['truePz'][evt_start_indices])))
        fout.create_earray(root, "input_slice", obj=np.column_stack((df['recoNu_E'][evt_start_indices], df['recoVertex_X'][evt_start_indices], df['recoVertex_Y'][evt_start_indices], df['recoVertex_Z'][evt_start_indices])))
        fout.create_earray(root, "isNonFlux", obj=isflux)
        for cvn_var in cvn_vars:
            fout.create_earray(root, cvn_var, obj=df[cvn_var][evt_start_indices])
            
    else:
        getattr(root, "mc.inter").append(inter)
        getattr(root, "tpc_id").append(df['tpc_id'][evt_start_indices])
        getattr(root, "trueE").append(df['trueEnergy'][evt_start_indices])
        getattr(root, "trueVertex").append(np.column_stack((df['trueVertex_X'][evt_start_indices],df['trueVertex_Y'][evt_start_indices], df['trueVertex_Z'][evt_start_indices])))
        getattr(root, "trueP").append(np.column_stack((df['truePx'][evt_start_indices], df['truePy'][evt_start_indices], df['truePz'][evt_start_indices])))
        getattr(root, "input_slice").append(np.column_stack((df['recoNu_E'][evt_start_indices], df['recoVertex_X'][evt_start_indices], df['recoVertex_Y'][evt_start_indices], df['recoVertex_Z'][evt_start_indices])))
        getattr(root, "isNonFlux").append(isflux)
        for cvn_var in cvn_vars:
            getattr(root, cvn_var).append(df[cvn_var][evt_start_indices])
    
    total_events+=len(events)
    files+=1
    print(files)
    print(total_events)
    
  fout.create_earray(root, "cvnmap_shape", obj=np.array([total_events, 3, 400, 280]))
  fout.create_earray(root, "prong_cvnmap_shape", obj=np.array([total_events, 20, 3, 400, 280]))
  
  fout.close()

if __name__ == '__main__':
    if len(sys.argv) < 3:
        sys.exit(f"Usage: {sys.argv[0]} OUTPUT.h5 INPUT.h5 [INPUT.h5 ...]")

    fout = sys.argv[1]
    if os.path.exists(fout):
        sys.exit(f"{fout} already exists. Will not overwrite")

    fin = []
    for pattern in sys.argv[2:]:
        matches = [path for path in glob.glob(pattern) if os.path.isfile(path)]
        if not matches:
            sys.exit(f"No input files found: {pattern}")
        fin.extend(matches)

    print(fout, fin)
    main(fout, fin)
