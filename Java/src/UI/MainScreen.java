package UI;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by slava_semeniuk on 3/17/17.
 */
public class MainScreen {
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

    public MainScreen() {
        for (JButton button : mainButtons) button.addActionListener(mainButtonActionListener);
        for (JButton button : secondaryButtons) button.addActionListener(secondaryActionListener);
    };

    private ActionListener mainButtonActionListener = new ActionListener() {
        public void actionPerformed(ActionEvent e) {
            JButton primaryButton = (JButton) e.getSource();
            int index = mainButtons.indexOf(primaryButton);
            JButton secondaryButton = secondaryButtons.get(index);

            if (secondaryButton.isSelected()) return;

            primaryButton.setSelected(!primaryButton.isSelected());
            secondaryButton.setEnabled(primaryButton.isSelected());

            primaryButton.setText(primaryButton.isSelected() ? "З" : "Д");
        }
    };

    private ActionListener secondaryActionListener = new ActionListener() {
        public void actionPerformed(ActionEvent e) {
            JButton secondaryButton = (JButton) e.getSource();
            int index = mainButtons.indexOf(secondaryButton);
            if (secondaryButton.isSelected()) {
                secondaryButton.setSelected(false);
                indexOfSelectedSecondaryButton = null;
                return;
            }
            if (indexOfSelectedSecondaryButton == null) {
                indexOfSelectedSecondaryButton = index;
                secondaryButton.setSelected(true);
                return;
            }
            return;
        }
    };

    public static void main(String[] args) {
        JFrame frame = new JFrame("MainScreen");
        frame.setContentPane(new MainScreen().mainPanel);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.pack();
        Dimension dimension = Toolkit.getDefaultToolkit().getScreenSize();
        frame.setLocation((int) dimension.getWidth(), (int) dimension.getHeight());
        frame.setAlwaysOnTop(true);
        frame.setResizable(false);
        frame.setVisible(true);
    }
}
