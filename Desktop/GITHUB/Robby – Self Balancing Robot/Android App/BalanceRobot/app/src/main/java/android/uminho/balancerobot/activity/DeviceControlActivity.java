package android.uminho.balancerobot.activity;

import android.app.ActionBar;
import android.app.Activity;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.preference.PreferenceManager;
import android.uminho.balancerobot.DeviceData;
import android.uminho.balancerobot.R;
import android.uminho.balancerobot.Utils;
import android.uminho.balancerobot.bluetooth.DeviceConnector;
import android.uminho.balancerobot.bluetooth.DeviceListActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import java.lang.ref.WeakReference;

public final class DeviceControlActivity extends BaseActivity {
    private static final String DEVICE_NAME = "DEVICE_NAME";
    private static final String LOG = "LOG";
    private static String MSG_NOT_CONNECTED;
    private static String MSG_CONNECTING;
    private static String MSG_CONNECTED;
    private static DeviceConnector connector;
    private static BluetoothResponseHandler mHandler;
    private StringBuilder logHtml;
    private static final String[] KName = {"Balance Kp", "Balance Ki", "Balance Kd", "Distance Kp", "Distance Ki", "Distance Kd"};
    private static final int[] ValIdArray = {R.id.BalancePValue, R.id.BalanceIValue, R.id.BalanceDValue, R.id.DistancePValue, R.id.DistanceIValue, R.id.DistanceDValue};
    private static final int[] InputIdArray ={R.id.Balance_P_Input, R.id.Balance_I_input, R.id.Balance_D_Input, R.id.Distance_P_Input, R.id.Distance_I_Input, R.id.Distance_D_Input};
    private TextView[] Val = new TextView[ValIdArray.length];
    private EditText[] Input = new EditText[InputIdArray.length];
    private String command_ending = "\n";
    private String deviceName;
    private String sendCommands = "";
    private Button BtnUp, BtnDown, BtnLeft, BtnRight, BtnSend;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        PreferenceManager.setDefaultValues(this, R.xml.settings_activity, false);


      //  setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        if (mHandler == null) mHandler = new BluetoothResponseHandler(this);
        else mHandler.setTarget(this);

        MSG_NOT_CONNECTED = getString(R.string.msg_not_connected);
        MSG_CONNECTING = getString(R.string.msg_connecting);
        MSG_CONNECTED = getString(R.string.msg_connected);

        setContentView(R.layout.activity_terminal);
        if (isConnected() && (savedInstanceState != null)) {
            setDeviceName(savedInstanceState.getString(DEVICE_NAME));
        } else getActionBar().setSubtitle(MSG_NOT_CONNECTED);

        this.logHtml = new StringBuilder();
        if (savedInstanceState != null) this.logHtml.append(savedInstanceState.getString(LOG));

        for (int i=0; i<ValIdArray.length; i++) {
            final int b = i;
            Val[b] = (TextView) findViewById(ValIdArray[b]);
        }

        for (int i=0; i<InputIdArray.length; i++) {
            final int b = i;
            Input[b] = (EditText) findViewById(InputIdArray[b]);
        }

        BtnUp = (Button) findViewById(R.id.UpButton);
        BtnDown = (Button) findViewById(R.id.DownButton);
        BtnLeft = (Button) findViewById(R.id.LeftButton);
        BtnRight = (Button) findViewById(R.id.RightButton);
        BtnSend = (Button) findViewById(R.id.ButtonSend);

        BtnSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendCommand(v);
            }
        });

        BtnUp.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch(event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        // PRESSED
                        v.setPressed(true);
                        disableOtherButtons(v);
                        directionCommand(v);
                        break;
                    case MotionEvent.ACTION_UP:
                        // RELEASED
                        v.setPressed(false);
                        enableOtherButtons(v);
                        stopCommand();
                        break; // if you want to handle the touch event
                }
                return true;
            }
        });
        BtnDown.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch(event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        // PRESSED
                        v.setPressed(true);
                        disableOtherButtons(v);
                        directionCommand(v);
                        break;
                    case MotionEvent.ACTION_UP:
                        // RELEASED
                        v.setPressed(false);
                        enableOtherButtons(v);
                        stopCommand();
                        break; // if you want to handle the touch event
                }
                return true;
            }
        });
        BtnRight.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch(event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        // PRESSED
                        v.setPressed(true);
                        disableOtherButtons(v);
                        directionCommand(v);
                        break;
                    case MotionEvent.ACTION_UP:
                        // RELEASED
                        v.setPressed(false);
                        enableOtherButtons(v);
                        stopCommand();
                        break; // if you want to handle the touch event
                }
                return true;
            }
        });
        BtnLeft.setOnTouchListener(new View.OnTouchListener() {
            @Override
            public boolean onTouch(View v, MotionEvent event) {
                switch(event.getAction()) {
                    case MotionEvent.ACTION_DOWN:
                        // PRESSED
                        v.setPressed(true);
                        disableOtherButtons(v);
                        directionCommand(v);
                        break;
                    case MotionEvent.ACTION_UP:
                        // RELEASED
                        v.setPressed(false);
                        enableOtherButtons(v);
                        stopCommand();
                        break; // if you want to handle the touch event
                }
                return true;
            }
        });

        /* (int i=0; i<Input.length; i++) {
            final int b = i;
            Input[b].setText("0" + String.valueOf(b+1));
        }*/
    }

    // ==========================================================================

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putString(DEVICE_NAME, deviceName);

    }
    // ============================================================================


    private boolean isConnected() {
        return (connector != null) && (connector.getState() == DeviceConnector.STATE_CONNECTED);
    }
    // ==========================================================================



    private void stopConnection() {
        if (connector != null) {
            connector.stop();
            connector = null;
            deviceName = null;
        }
    }
    // ==========================================================================



    private void startDeviceListActivity() {
        stopConnection();
        Intent serverIntent = new Intent(this, DeviceListActivity.class);
        startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
    }
    // ============================================================================



    @Override
    public boolean onSearchRequested() {
        if (super.isAdapterReady()) startDeviceListActivity();
        return false;
    }
    // ==========================================================================


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.device_control_activity, menu);
        final MenuItem bluetooth = menu.findItem(R.id.menu_search);
        if (bluetooth != null) bluetooth.setIcon(this.isConnected() ?
                R.drawable.ic_action_device_bluetooth_connected :
                R.drawable.ic_action_device_bluetooth);
        return true;
    }
    // ============================================================================


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {

            case R.id.menu_search:
                if (super.isAdapterReady()) {
                    if (isConnected()) stopConnection();
                    else startDeviceListActivity();
                } else {
                    Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                    startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
                }
                return true;

            case R.id.menu_power:
                if (isConnected()){
                    DialogInterface.OnClickListener dialogClickListener = new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            switch (which){
                                case DialogInterface.BUTTON_POSITIVE:
                                    //Yes button clicked
                                    String commandString = "o\n";
                                    byte[] command = commandString.getBytes();
                                    connector.write(command);
                                    break;

                                case DialogInterface.BUTTON_NEGATIVE:
                                    //No button clicked
                                    break;
                            }
                        }
                    };
                    AlertDialog.Builder builder = new AlertDialog.Builder(this);
                    builder.setMessage("Are you sure?").setPositiveButton("Yes", dialogClickListener)
                            .setNegativeButton("No", dialogClickListener).show();
                }
                else
                    Toast.makeText(getApplicationContext(), "Not Connected", Toast.LENGTH_SHORT).show();

            case R.id.menu_auto:
                if (isConnected()) {
                    String commandString = "a\n";
                    byte[] command = commandString.getBytes();
                    connector.write(command);
                    Toast.makeText(getApplicationContext(), "Automatic PID tuning requested", Toast.LENGTH_SHORT).show();
                }
                else
                    Toast.makeText(getApplicationContext(), "Not Connected", Toast.LENGTH_SHORT).show();
            default:
                return super.onOptionsItemSelected(item);
        }
    }
    // ============================================================================


    @Override
    public void onStart() {
        super.onStart();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        switch (requestCode) {
            case REQUEST_CONNECT_DEVICE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) {
                    String address = data.getStringExtra(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                    BluetoothDevice device = btAdapter.getRemoteDevice(address);
                    if (super.isAdapterReady() && (connector == null)) setupConnector(device);
                }
                break;
            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                super.pendingRequestEnableBt = false;
                if (resultCode != Activity.RESULT_OK) {
                    Utils.log("BT not enabled");
                }
                break;
        }
    }
    // ==========================================================================



    private void setupConnector(BluetoothDevice connectedDevice) {
        stopConnection();
        try {
            String emptyName = getString(R.string.empty_device_name);
            DeviceData data = new DeviceData(connectedDevice, emptyName);
            connector = new DeviceConnector(data, mHandler);
            connector.connect();
        } catch (IllegalArgumentException e) {
            Utils.log("setupConnector failed: " + e.getMessage());
        }
    }
    // ==========================================================================


    public void receiveCommand(String message) {
        message = message.replace("\r", "").replace("\n", "");
        String parts[] = message.split("-");

        for(int i = 0; i < parts.length; i++) {
            String firstChar = parts[i].substring(0, 1);
            String value = parts[i].substring(1);
            int num = Integer.parseInt(firstChar);
            //char command[] = parts[i].toCharArray();
            //int num = command[0];

            Val[num-1].setText(value);
        }
        Toast.makeText(getApplicationContext(), "New PID values received", Toast.LENGTH_SHORT).show();

       /* if (command[0] == 'p'){
            mode = 1;
            num = 0;
        }
        if (mode == 1){
                Val[num].setText(message);
                num++;
            if(num == Val.length){
                mode = 0;
                num = 0;
            }
        }*/
    }

    public void sendCommand(View view) {

        if (isConnected()) {
            sendCommands = sendCommands.concat("0");
            for (int i = 0; i < Input.length; i++) {
                String val = Input[i].getText().toString();
                if (!val.isEmpty()) {
                    if(Float.parseFloat(val) < 200)
                        sendCommands = sendCommands.concat(String.valueOf((i+1)) + val + "-");
                    else
                        Toast.makeText(getApplicationContext(), "Value of box " + KName[i] + " is too big", Toast.LENGTH_SHORT).show();
                }
                Input[i].setText("");
            }
            if (sendCommands != "0") {
                //sendCommands = sendCommands.substring(0, sendCommands.length()-1);
                sendCommands = sendCommands.concat("+");
                byte[] command = sendCommands.getBytes();
                if (command_ending != null)
                    command = Utils.concat(command, command_ending.getBytes());
                connector.write(command);
                Toast.makeText(getApplicationContext(), "PID values sent", Toast.LENGTH_SHORT).show();
            }
            else
                Toast.makeText(getApplicationContext(), "Nothing to send", Toast.LENGTH_SHORT).show();
            sendCommands = "";
        }
         else
            Toast.makeText(getApplicationContext(), "Not Connected", Toast.LENGTH_SHORT).show();
    }

    public void directionCommand(View view){
        String commandString;
        byte[] command;

        if (isConnected()) {
            switch (view.getId()){
                case R.id.UpButton:
                    commandString = "u\n";
                    command = commandString.getBytes();
                    connector.write(command);
                    break;

                case R.id.DownButton:
                    commandString = "d\n";
                    command = commandString.getBytes();
                    connector.write(command);
                    break;
                case R.id.RightButton:
                    commandString = "r\n";
                    command = commandString.getBytes();
                    connector.write(command);
                    break;
                case R.id.LeftButton:
                    commandString = "l\n";
                    command = commandString.getBytes();
                    connector.write(command);
                    break;
            }
        }
        else
            Toast.makeText(getApplicationContext(), "Not Connected", Toast.LENGTH_SHORT).show();
    }

    public void stopCommand(){
        if (isConnected()) {
            String commandString = "s\n";
            byte[] command = commandString.getBytes();
            connector.write(command);
        }
    }
    void disableOtherButtons(View view){
        if(view.getId()!= R.id.UpButton)
            BtnUp.setEnabled(false);
        if(view.getId()!= R.id.DownButton)
            BtnDown.setEnabled(false);
        if(view.getId()!= R.id.RightButton)
            BtnRight.setEnabled(false);
        if(view.getId()!= R.id.LeftButton)
            BtnLeft.setEnabled(false);
        BtnSend.setEnabled(false);
    }
    void enableOtherButtons(View view){
        if(view.getId()!= R.id.UpButton)
            BtnUp.setEnabled(true);
        if(view.getId()!= R.id.DownButton)
            BtnDown.setEnabled(true);
        if(view.getId()!= R.id.RightButton)
            BtnRight.setEnabled(true);
        if(view.getId()!= R.id.LeftButton)
            BtnLeft.setEnabled(true);
        BtnSend.setEnabled(true);
    }

    // ==========================================================================


    void setDeviceName(String deviceName) {
        this.deviceName = deviceName;
        getActionBar().setSubtitle(deviceName);
    }
    // ==========================================================================

    private static class BluetoothResponseHandler extends Handler {
        private WeakReference<DeviceControlActivity> mActivity;

        public BluetoothResponseHandler(DeviceControlActivity activity) {
            mActivity = new WeakReference<DeviceControlActivity>(activity);
        }

        public void setTarget(DeviceControlActivity target) {
            mActivity.clear();
            mActivity = new WeakReference<DeviceControlActivity>(target);
        }

        @Override
        public void handleMessage(Message msg) {
            DeviceControlActivity activity = mActivity.get();
            if (activity != null) {
                switch (msg.what) {
                    case MESSAGE_STATE_CHANGE:

                        Utils.log("MESSAGE_STATE_CHANGE: " + msg.arg1);
                        final ActionBar bar = activity.getActionBar();
                        switch (msg.arg1) {
                            case DeviceConnector.STATE_CONNECTED:
                                bar.setSubtitle(MSG_CONNECTED);
                                String commandString = "p\n";
                                byte[] command = commandString.getBytes();
                                connector.write(command);
                                break;
                            case DeviceConnector.STATE_CONNECTING:
                                bar.setSubtitle(MSG_CONNECTING);
                                break;
                            case DeviceConnector.STATE_NONE:
                                bar.setSubtitle(MSG_NOT_CONNECTED);
                                break;
                        }
                        activity.invalidateOptionsMenu();
                        break;


                    case MESSAGE_DEVICE_NAME:
                        activity.setDeviceName((String) msg.obj);
                        break;

                    case MESSAGE_READ:
                        final String readMessage = (String) msg.obj;
                        if (readMessage != null) {
                            activity.receiveCommand(readMessage);
                        }
                        break;

                    case MESSAGE_WRITE:
                        // stub
                        break;

                    case MESSAGE_TOAST:
                        // stub
                        break;
                }
            }
        }
    }
    // ==========================================================================
}