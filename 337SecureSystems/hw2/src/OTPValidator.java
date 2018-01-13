import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class OTPValidator extends JFrame implements ActionListener{
    private JFrame frame;
    private JButton button;
    private JPanel panel;
    private JTextField text;
    private JTextField successField;
    private OTPGenerator generator;
    private boolean synchronizing;
    private int synchOffset;

    public OTPValidator() {
        //create SHA-256 password generator
        generator = new OTPGenerator();
        //create JFrame
        frame = new JFrame("PasswordChecker");

        setTitle("Password Checker");
        setSize(400,100);

        //Create JButton and JPanel
        button = new JButton("Check password");
        panel = new JPanel();
        text = new JTextField(6);
        successField = new JTextField(20);

        //Add button and textfield to JPanel
        panel.add(text);
        panel.add(button);
        panel.add(successField);
        //Add JPanel to JFrame
        this.getContentPane().add(panel);

        //add an actionListener to button
        button.addActionListener(this);

        //a flag for actionPerformed() that is executed if we are attempting to synchronize
        //the validator and generator
        synchronizing = false;
        //represents how many hashes the expected OTP and input OTP are
        synchOffset = 0;


        successField.setEditable(false);
        setVisible(true);
        setResizable(false);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }

    /**
     * checks if input OTP string is equal to current generator's OTP string
     * @param in
     * @return
     */
    private boolean validateOTP(String in) {
        String expectedOTP = generator.getOTP();
        String inputOTP = in;
        if(expectedOTP.equals(inputOTP)) {
            return true;
        }
        else {
            return false;
        }
    }

    /**
     * Checks the next 1000 hashes for a matching input.
     *
     * @return
     */
    private boolean checkFutureOTP(String inOTP) {
        OTPGenerator newGen = new OTPGenerator(generator.getHash());
        for(int i = 1; i <= 100; i++) {
            if(newGen.getOTP().equals(inOTP)) {
                synchOffset = i;
                return true;
            }
            else {
                newGen.generateNextHash();
            }
        }
        return false;
    }
    private boolean validateFutureOTP(String inOTP) {
        OTPGenerator newGen = new OTPGenerator(generator.getHash());
        //check if input OTP is equal to the OTP at the synchronization offset + 1
        for(int i = 0; i < synchOffset; i++) {
            newGen.generateNextHash();
        }
        if(newGen.getOTP().equals(inOTP)) {
            //replace the OTP generator with the new, more current one
            generator = newGen;
            return true;
        }
        else{
            return false;
        }
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String inputOTP = text.getText();
        //when we are checking next two OTP in sequence
        if(synchronizing) {
            boolean correct = validateFutureOTP(inputOTP);
            if(correct) {
                //OTP at future OTP + 1 is a match
                successField.setText("SUCCESS");
                generator.generateNextHash();
                synchronizing = false;
            }
            else {
                //OTP at future OTP + 1 is not a match
               successField.setText("FAILED");
               synchronizing = false;
            }
        }
        else { //otherwise, check the immediate OTP
            boolean correct = validateOTP(inputOTP);
            //input OTP matches expected OTP
            if(correct) {
                successField.setText("SUCCESS");
                generator.generateNextHash();
            }
            //input OTP does not match expected OTP
            else {
                boolean isMatch = checkFutureOTP(inputOTP); //???
                if(isMatch) {
                    //input OTP matches a future OTP in sequence
                    successField.setText("INPUT NEXT OTP");
                    synchronizing = true;
                }
                //input OTP does not match a future OTP
                else {
                    successField.setText("FAILED");
                }
            }
        }


    }
    public static void main(String[] args) {
        OTPValidator app = new OTPValidator();
    }


}
