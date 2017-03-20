package UI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import com.fazecast.jSerialComm.SerialPort;

import java.util.ArrayList;
import java.util.Arrays;

import com.sun.org.apache.xpath.internal.operations.Bool;
import command.*;
import connection.*;

import static connection.SerialConnection.*;


/**
 * Created by slava_semeniuk on 3/17/17.
 */
public class MainScreen implements IOnSendListener, IOnReceiveListener {
    private JButton allButton;
    private JPanel mainPanel;
    private JButton secondMainRelayButton;
    private JButton thirdMainRelayButton;
    private JButton firstMainRelayButton;
    private JButton fifthMainRelayButton;
    private JButton seventhMainRelayButton;
    private JButton sixthMainRelayButton;
    private JButton firstRelayButton;
    private JButton secondRelayButton;
    private JButton thirdRelayButton;
    private JButton fourthRelayButton;
    private JButton fifthRelayButton;
    private JButton sixthRelayButton;
    private JButton seventhRelayButton;
    private JButton eightRelayButton;
    private JButton eightMainRelayButton;
    private JButton offButton;
    private JButton fourthMainRelayButton;
    private Integer indexOfSelectedSecondaryButton;

    private Connection connection;
    private Parser commandParser = new Parser();

    private ArrayList<JButton> mainButtons = new ArrayList<JButton>(Arrays.asList(
            firstMainRelayButton, secondMainRelayButton,
            thirdMainRelayButton, fourthMainRelayButton,
            fifthMainRelayButton, sixthMainRelayButton,
            seventhMainRelayButton, eightMainRelayButton));

    private ArrayList<JButton> secondaryButtons = new ArrayList<JButton>(Arrays.asList(
            firstRelayButton, secondRelayButton,
            thirdRelayButton, fourthRelayButton,
            fifthRelayButton, sixthRelayButton,
            seventhRelayButton, eightRelayButton));


    public static void main(String[] args) {
        JFrame frame = new JFrame("MainScreen");
        frame.setContentPane(new MainScreen().mainPanel);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
        frame.setLocation((int) dimension.getWidth() - frame.getWidth(), (int) dimension.getHeight() - frame.getHeight());
        frame.setAlwaysOnTop(true);
        frame.setResizable(false);
        frame.setVisible(true);
    }

    public MainScreen() {
        for (JButton button : mainButtons) button.addActionListener(mainButtonActionListener);
        for (JButton button : secondaryButtons) button.addActionListener(secondaryActionListener);
        allButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
            }
        });
        connect();
    };

    private ActionListener mainButtonActionListener = new ActionListener() {
        public void actionPerformed(ActionEvent e) {
            JButton primaryButton = (JButton) e.getSource();
            int index = mainButtons.indexOf(primaryButton);
            boolean state = !primaryButton.isSelected();
            sendActionForRelay(0,state,index);
        }
    };

    private ActionListener secondaryActionListener = new ActionListener() {
        public void actionPerformed(ActionEvent e) {
            JButton button = (JButton) e.getSource();
            int index = secondaryButtons.indexOf(button);
            boolean state = !button.isSelected();
            sendActionForRelay(1,state,index);
        }
    };

    private void sendActionForRelay(int group, boolean state, int number) {
        Command command = new Command(Commands.TURN);
        command.addArgument(new Argument((byte) 'g', group));
        command.addArgument(new Argument((byte) 's', (state) ? 0 : 1));
        command.addArgument(new Argument((byte) 'n', number));
        if (connection != null) connection.send(command.serialize());
    }


    private void sendActionForAllRelays(Boolean state) {
        for (int index = 0; index < mainButtons.size(); index++) {
            JButton mainButton = mainButtons.get(index);
            mainButton.setSelected(state);
            mainButton.setText(state ? "З" : "Д");
            JButton secondaryButton = secondaryButtons.get(index);
            secondaryButton.setEnabled(state);
            if (!state) secondaryButton.setSelected(false);
        }
    }

    private void turnAll(Boolean state) {
        for (JButton button : mainButtons) {
            button.setSelected(state);
        };
    }

    private void turn(int group, boolean state, int number) {
        String buttonText = state ? "З" : "Д";
        if (group == 1) buttonText = "В";
        ArrayList<JButton> buttonList = (group == 1 ? secondaryButtons : mainButtons);
        buttonList.get(number).setSelected(state);
        buttonList.get(number).setText(buttonText);
        if (group == 0) secondaryButtons.get(number).setEnabled(state);
    }

    private void connect() {
        if (connection != null) connection.close();
        SerialPort port = SerialPort.getCommPorts()[0];
        connection = new SerialConnection(port, 9600);
        if (!connection.isOpen() || connection == null) {
            JOptionPane.showMessageDialog(null, "Не вдалось встановити зв'язок");
            return;
        }

        connection.addOnSendListener(this);
        connection.addOnReceiveListener(this);
    }

    protected void disconnect() {
        if (connection != null) connection.close();
    }

    @Override
    public void onSend(byte[] data) {}

    @Override
    public void onReceive(byte[] data) {
        System.out.print(data);
        for (Command command : commandParser.parse(data)) {
            System.out.print(command);
            try {
                boolean state = command.getArgument(Commands.STATE).getBoolean();
                switch (command.getKey()) {
                    case Commands.TURN:
                        int number = command.getArgument(Commands.NUMBER).getShort();
                        int group = command.getArgument(Commands.GROUP).getShort();
                    case Commands.TURN_ALL:
                        turnAll(state);
                }
            } catch (Exception e) {
                System.out.print(e);
                e.printStackTrace();
            }
        }
    }
}
