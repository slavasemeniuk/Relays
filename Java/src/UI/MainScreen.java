package UI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import com.fazecast.jSerialComm.SerialPort;

import java.util.ArrayList;
import java.util.Arrays;

import command.*;
import connection.*;

import static connection.SerialConnection.*;


/**
 * Created by slava_semeniuk on 3/17/17.
 */
public class MainScreen implements IOnSendListener, IOnReceiveListener {
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
    private JButton fourthMainRelayButton;

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
        frame.setTitle("VidManager");
    }

    public MainScreen() {
        for (JButton button : secondaryButtons) {
            button.addActionListener(secondaryActionListener);
            button.setBackground(Color.decode("#3797d6"));
            button.setForeground(Color.black);
        }
        for (JButton button : mainButtons) {
            button.addActionListener(mainButtonActionListener);
            button.setBackground(Color.decode("#3797d6"));
            button.setForeground(Color.black);
        };
        connect();
        sendActionGetStates();

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

    private void turnButton(int group, boolean state, int number) {
        String buttonText = state ? "З" : "Д";
        if (group == 1) buttonText = "В";
        ArrayList<JButton> buttonList = (group == 1 ? secondaryButtons : mainButtons);
        buttonList.get(number).setSelected(state);
        buttonList.get(number).setText(buttonText);
        if (group == 0) secondaryButtons.get(number).setEnabled(state);
        if (group == 0) {
            buttonList.get(number).setBackground(state ? Color.decode("#e1e1e1") : Color.decode("#3797d6"));
            buttonList.get(number).setForeground(state ? Color.decode("#000ff") : Color.black);
        } else {
            secondaryButtons.get(number).setBackground(state ? Color.decode("#2ba834") : Color.decode("#3797d6"));
            secondaryButtons.get(number).setForeground(state ? Color.white : Color.black);
        }
    }

    private void connect() {
        if (connection != null) connection.close();
        SerialPort[] availablePorts = SerialPort.getCommPorts();
        SerialPort selectedPort = null;

        selectedPort = SerialPort.getCommPort("COM17");

        if (selectedPort == null) {
            JOptionPane.showMessageDialog(null, "Не знайдено COM17");
            return;
        }
        connection = new SerialConnection(selectedPort, 9600);
        if (connection == null || !connection.isOpen()) {
            JOptionPane.showMessageDialog(null, "Не вдалось підключитись да COM17");
            return;
        }
        connection.addOnSendListener(this);
        connection.addOnReceiveListener(this);
    }


    private void disconnect() {
        if (connection != null) connection.close();
    }

    @Override
    public void onSend(byte[] data) {}

    @Override
    public void onReceive(byte[] data) {
        for (Command command : commandParser.parse(data)) {
            try {
                switch (command.getKey()) {
                    case Commands.TURN:
                        boolean state = command.getArgument(Commands.STATE).getBoolean();
                        int number = command.getArgument(Commands.NUMBER).getShort();
                        int group = command.getArgument(Commands.GROUP).getShort();
                        turnButton(group, state, number);
                }
            } catch (Exception e) {
                System.out.print(e);
                e.printStackTrace();
            }
        }
    }

    private void sendActionForRelay(int group, boolean state, int number) {
        Command command = new Command(Commands.TURN);
        command.addArgument(new Argument((byte) 'g', group));
        command.addArgument(new Argument((byte) 'o', state));
        command.addArgument(new Argument((byte) 'n', number));
        if (connection != null) connection.send(command.serialize());
    }

    private void sendActionGetStates() {
        Command command = new Command(Commands.GET_STATES);
        if (connection != null) connection.send(command.serialize());
    }
}
