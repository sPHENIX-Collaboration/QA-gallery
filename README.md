# QA-gallery

Plotting macro and result gallery for standardized QA plots

## QA branches

The `main` branch carries common macros and utilities. And each QA plot setup is placed in their individual branches: 

* [`QA-calorimeter-single-particle` branch](https://github.com/sPHENIX-Collaboration/QA-gallery/tree/QA-calorimeter-single-particle ) : Full detector Calorimeter QA for single particles, designed to plot QA file from https://github.com/sPHENIX-Collaboration/macros/tree/QA-calorimeter-single-particle/detectors/sPHENIX
* [`QA-tracking-low-occupancy` branch](https://github.com/sPHENIX-Collaboration/QA-gallery/tree/QA-tracking-low-occupancy ) : Tracking QA for low occupancy, designed to plot QA file from https://github.com/sPHENIX-Collaboration/macros/tree/QA-tracking-low-occupancy/detectors/sPHENIX
* [`QA-tracking-high-occupancy` branch](https://github.com/sPHENIX-Collaboration/QA-gallery/tree/QA-tracking-high-occupancy ) : Tracking QA for high occupancy, designed to plot QA file from https://github.com/sPHENIX-Collaboration/macros/tree/QA-tracking-high-occupancy/detectors/sPHENIX
* [`QA-tracking-pythiajet` branch](https://github.com/sPHENIX-Collaboration/QA-gallery/tree/QA-tracking-pythiajet ) : Pythia D0-jet events with a Upsilon embedding, designed to plot QA file from https://github.com/sPHENIX-Collaboration/macros/tree/QA-tracking-pythiajet/detectors/sPHENIX

The plotting branches are automatically synced with the `main` branch via GitHub actions. 

## Result plots

The result by running these QA notebooks are archived by tags. Please find a list of recent runs at https://github.com/sPHENIX-Collaboration/QA-gallery/tags 

# Developper's guide

For base script and [`QA_Draw_Utility.C`](QA_Draw_Utility.C), please edit in the `main` branch. Then it will be automatically sync to other plotting branches via [GitHub Actions](https://github.com/sPHENIX-Collaboration/QA-gallery/actions)

In the plotting branches, the plotting macros plot the default QA ROOT file via the sPHENIX Jupyter Lab environment. Both source code, documentation and result plots are saved in the Jupyter Notebook formats, such as [QA-calorimeter.ipynb](https://github.com/sPHENIX-Collaboration/QA-gallery/blob/QA-calorimeter-single-particle/QA-calorimeter.ipynb) and they can be best viewed via the `nbviewer` site ([example](http://nbviewer.ipython.org/github/sPHENIX-Collaboration/QA-gallery/blob/cb340543f08c4351fcf5787de1c495419ea2debd/QA-calorimeter.ipynb))

To edit the notebook, 
1. Please checkout the plotting branches
2. Generate or download the QA ROOT file from a recent QA runs, e.g. [Calorimeter QA jobs](https://web.sdcc.bnl.gov/jenkins-sphenix/job/sPHENIX/job/test-calo-single-qa/)
3. Edit/execute in either with following environements: 

* sPHENIX JupyterLab via SDCC HTC Jupyter Hub, which provides both latest software and computing power. Here is an introduction: https://github.com/sPHENIX-Collaboration/tutorials/tree/master/JupyterLab

<p  align="center">
<a href = "https://jupyter.sdcc.bnl.gov/jupyterhub/htc"><img  src="https://jupyter.sdcc.bnl.gov/images/jupyterhub-logo-card-htc.png" alt="SDCC HTC Jupyter Hub"
	title="SDCC HTC Jupyter Hub" height="200" />
</a></p>

* sPHENIX JupyterLab runs locally on your local computer via [sPHENIX Singularity container](https://github.com/sPHENIX-Collaboration/Singularity). Please setup the [container](https://github.com/sPHENIX-Collaboration/Singularity), source sPHENIX setup, and run [`setup.sh`](https://github.com/sPHENIX-Collaboration/QA-gallery/blob/QA-calorimeter-single-particle/setup.sh) to start the sPHENIX [JupyterLab webserver interface](https://jupyterlab.readthedocs.io/). 

![JupyterLab screenshot](material/JupyterLab-screenshot.png)

4. Last step, before making a [pull request](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request), please execute the "Edit/clear all output" command in JupyterLab. It will clear out output and all for a clean git diff for the pull request
