# dsdlc_generated

See https://github.com/dronecan/dronecan_dsdlc for instructions on how to
generate the files found in here.
The instructions are something along the lines of:
```sh
python3 -m venv .venv
source .venv/bin/activate
pip install empy==3.3.4 pexpect
git clone git@github.com:dronecan/dronecan_dsdlc.git
git clone git@github.com:dronecan/pydronecan.git
git clone git@github.com:dronecan/DSDL.git
git clone git@github.com:dronecan/libcanard.git
python dronecan_dsdlc/dronecan_dsdlc.py -O dsdlc_generated DSDL/uavcan
```

From there, the necessary files can be copied from inside `dsdlc_generated`.



Data types:
* [`uavcan.equipment.power.BatteryInfo`](https://dronecan.github.io/Specification/7._List_of_standard_data_types/#batteryinfo).