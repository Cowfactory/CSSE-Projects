import javax.swing.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class OTPApplication extends JFrame implements ActionListener{
    private JFrame frame;
    private JButton button;
    private JPanel panel;
    private JTextField text;
    OTPGenerator generator;

    public OTPApplication() {
        //create SHA-256 password generator
        generator = new OTPGenerator();
        //create JFrame
        frame = new JFrame("Password Generator");

        setTitle("Password Generator");
        setSize(200,100);

        //Create JButton and JPanel
        button = new JButton("Generate new Password");
        panel = new JPanel();
        text = new JTextField(6);

        //Add button and textfield to JPanel
        panel.add(button);
        panel.add(text);
        //Add JPanel to JFrame
        this.getContentPane().add(panel);

        //add an actionListener to button
        button.addActionListener(this);

        //Populate textbox with initial OTP
        text.setText(generator.getOTP());

        setVisible(true);
        setResizable(false);
        setDefaultCloseOperation(EXIT_ON_CLOSE);
    }
    @Override
    public void actionPerformed(ActionEvent e) {
        generator.generateNextHash();
        String output = generator.getOTP();
        text.setText(output);
    }
    public static void main(String[] args) {
        OTPApplication app = new OTPApplication();
    }


}
