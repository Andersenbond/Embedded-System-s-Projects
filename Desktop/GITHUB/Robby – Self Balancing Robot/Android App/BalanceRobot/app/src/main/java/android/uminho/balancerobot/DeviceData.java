package android.uminho.balancerobot;

import java.util.ArrayList;

import android.bluetooth.BluetoothDevice;
import android.os.ParcelUuid;

import android.uminho.balancerobot.bluetooth.BluetoothUtils;


public class DeviceData {
    private String name = "";
    private String address = "";
    private int bondState = BluetoothDevice.BOND_NONE;
    private ArrayList<ParcelUuid> uuids = null;
    private final int deviceClass;
    private final int majorDeviceClass;

    public DeviceData(BluetoothDevice device, String emptyName) {
        name = device.getName();
        address = device.getAddress();
        bondState = device.getBondState();

        if (name == null || name.isEmpty()) name = emptyName;
        deviceClass = device.getBluetoothClass().getDeviceClass();
        majorDeviceClass = device.getBluetoothClass().getMajorDeviceClass();
        uuids = BluetoothUtils.getDeviceUuids(device);
    }

    public String getName() {
        return name;
    }

    public String getAddress() {
        return address;
    }
}
