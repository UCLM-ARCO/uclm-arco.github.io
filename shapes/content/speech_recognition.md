---
title: "Speech Recognition with DeepSpeech"
date: 2021-04-28T10:28:08+02:00
draft: false
tags: 
- voice assistant
- speech to text
- deepspeech

image: "speech_recognition/icon.png"
description: "A recipe to learn how train a speech recognition model and how to use that with DeepSpeech"
---

## Overview
DeepSpeech is a speech recognition engine developed by Mozilla that uses a model trained by machine learning techniques. This recipe will explain how to make use of this tool for speech recognition. 

## Ingredients
- Linux environment (tested on Ubuntu 20.04 LTS x64)
- APT dependencies: docker (>= 20.10.2), python3 (>= 3.7), venv
- The speech recognition repository available in [this link](https://bitbucket.org/cristibp11/shapes-mm-speechrecognition/src/master/)

## Setup
First, clone the repository provided in the **Ingredients** section. Once cloned you must copy the audios, in .wav format, that you want to convert from speech to text into a folder called **original**. In the repository you can find some sample audios in the **Downloads** section. 

{{<shell>}}
user@pc:~/$ tree original
original
└── es
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_1_001.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_1_002.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_1_003.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_1_004.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_2_001.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_2_002.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_2_003.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_3_001.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_3_002.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_3_003.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_4_001.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_4_002.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_4_003.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_5_001.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_5_002.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_6_001.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_6_002.wav
    ├── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_7_001.wav
    └── shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_7_002.wav

1 directory, 19 files
{{</shell>}}

## Set the commands
In the name of the audios an id is specified. This id has to match the content of the `commands.csv` file, which specifies the audio id and the audio transcript. With the example audios our `commands.csv` file would look like this: 
{{<shell>}}
user@pc:~/$ cat commands.csv
id,es
1,ok espejo
2,hola espejo
3,espejo
4,vale espejo
5,llama a mi hija
6,espejo llama a mi hijo
7,espejo llama a mi nuera
{{</shell>}}

## Usage to train a model
In order to install all dependencies, you must execute from the repository root directory `make dependencies`. Then, execute `make volumes` and finally `make`, which executes `build` and `run` targets. The audio will be processed and stored in a new folder calles **data** (this folder will be copied into a docker volume to be used by the container), and will be generated as `data/train.csv`, `registry.csv`, `Dockerfile`, and so on. Also, then container will start running detached. For checking its status execute `make log`. 

If you conciusly want to stop the container, use `make stop`. This stopping the container and stores last finished DeepSpeech epoch so, if you start again the container continues from there. If you want to restart the container just execute `make run`. 

In case the training of the model has finished and you want to restart it anyway, change in the file `configuration.env` the variables `CURRENT_EPOCH` to 0 or `MAX_EPOCH` to a value greater than the current one. 

If you manually added or changed something keep in mind that DeepSpeech could no longer work, as it tries to restore from a checkpoint that has invalid values. To start a new training, rebuild the trainer image executing `make rebuild NEW_DATA=yes LOAD_FROM_CHECKPOINT=no`. This copies the new and modified data contents to docker volume and tells DeepSpeech to not load from any checkpoint, so there is no errors. 

## Making a mmap-able model
The `shapes-mm-speechrecognition.pb` model file generated in the above step will be loaded in memory to be dealt with when running inference. This will results in extra loading time and memory consumption. One way to avoid this is to directly read data from the disk.

Tensorflow has tooling to achieve this, so clone DeepSpeech repository with the command `git clone --branch v0.9.3 https://github.com/mozilla/DeepSpeech` and use `util/taskcluster.py` tool to download:
{{<shell>}}
user@user-pc:~$ python3 util/taskcluster.py --source tensorflow --artifact convert_graphdef_memmapped_format --branch r1.15 --target .
{{</shell>}}

Then, from the directory where you will use the model execute the next command:
{{<shell>}}
user@user-pc:~$ ./convert_graphdef_memmapped_format --in_graph=/var/lib/docker/volumes/ds_model/_data/shapes-mm-speechrecognition.pb --out_graph=shapes-mm-speechrecognition.pbmm
{{</shell>}}


## Run the model
First, you will have to install the DeepSpeech wheel. To perform the installation, just use pip3 as such `pip3 install deepspeech`. After installation has finished, you should be able to call deepspeech from the next command-line:

{{<shell>}}
user@user-pc:~$ deepspeech --model shapes-mm-speechrecognition.pbmm --audio data/es/<name_file_audio>
{{</shell>}}

The output of this command with one of the example audios would look like this: 
{{<shell>}}
user@user-pc:~$ deepspeech --model shapes-mm-speechrecognition.pbmm --audio data/es/shapes_06d3eec8a7f1add24c69dcf04125c06b_2_3_000000_1_001.wav
Loading model from file shapes-mm-speechrecognition.pbmm
TensorFlow: v2.3.0-6-g23ad988
DeepSpeech: v0.9.3-0-gf2e9c85
2021-04-28 09:32:21.749436: I tensorflow/core/platform/cpu_feature_guard.cc:142] This TensorFlow binary is optimized with oneAPI Deep Neural Network Library (oneDNN)to use the following CPU instructions in performance-critical operations:  AVX2 FMA
To enable them in other operations, rebuild TensorFlow with the appropriate compiler flags.
Loaded model in 0.0949s.
Running inference.
ok espejo
Inference took 1.570s for 2.400s audio file.
{{</shell>}}