# Vuldeff
VULDEFF: Vulnerability Detection Method Based on Function Fingerprints and Code Differences
## Introduction
The vastly increased use of open source software has caused a rapid rise in software vulnerability area due to code cloning. Code similarity detection methods are usually used to detect vulnerabilities due to code cloning. But cloned code often modifies the original code to varying degrees, and the differences between the vulnerable code and the patched code can be very small. In order to effectively detect common mutation patterns in code clones and be able to distinguish vulnerable code from patched code, the paper proposes a vulnerability detection method named VULDEFF based on function fingerprints and code differences. This paper designs a lightweight function fingerprint method based on the Context Triggered Piecewise Hashing algorithm, which can characterize the basic syntax features of function source code. In particular, the fingerprint of the vulnerable function contains the syntax features, vulnerability features, and patch features of the vulnerable function, which can distinguish the vulnerable code from the patched code. VULDEFF detects whether the target function has vulnerabilities by searching for the target function fingerprint in the vulnerable function fingerprint library. Compared with five advanced software vulnerability detection tools, VULDEFF significantly reduces the false positive rate and false negative rate while ensuring the scalability of vulnerability detection. VULDEFF discovered 111 new vulnerabilities in 10 open source projects.
## Dataset
Due to the lack of large-scale, real, and uniformly formatted vulnerability datasets in the field of software security, we realize the automatic collection and processing of vulnerability patches and vulnerability source code and build a reliable and continuously updated patch and vulnerability source code dataset. The dataset includes 9,546 patch files of CVE and 24,920 unpatched vulnerable source code files, covering 464 programs, and can be extended to support other open source projects.

The complete data can be downloaded from [Zenodo](https://zenodo.org/records/11056403).



