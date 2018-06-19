# HPS_Analysis

A basic analysis using the ROOT dst from hps-dst.

The base class for the analysis is BaseAna. This class takes care of the nicities for running an analysis in ROOT. 
Just derive your own analysis from BaseAna and override the Process() and SlaveBegin() methods.

Features:
* Can be run with TProof on N processors simultaneously.
* Can be run from TChain::Process("MyClass()")
* Can be run with the Run() method.
** Makes it easier to debug your own analysis in a debugger by creating a mini application.

