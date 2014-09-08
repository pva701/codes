/**
 * Created by pva701 on 9/8/14.
 */

import java.io.*;

public class Main {
    static public void main(String[] args) throws IOException {
        File file = new File("input.txt" );

        BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream( file ), "UTF-8"));
        String line;
        int numRules = Integer.parseInt(br.readLine());
        String[] rules = new String[numRules];
        for (int i = 0; i < numRules; ++i)
            rules[i] = br.readLine();
        int numProof = Integer.parseInt(br.readLine());
        String[] proof = new String[numProof];
        for (int i = 0; i < numProof; ++i)
            proof[i] = br.readLine();
        br.close();

        Validator validator = new Validator(rules, proof);
        String[] result = validator.validate();
        for (int i = 0; i < result.length; ++i)
            System.out.println(result[i]);
    }
}
