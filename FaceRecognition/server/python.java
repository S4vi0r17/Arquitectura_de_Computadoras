public class CodigoGray {

    public static void main(String[] args) {
        String grayCode = "0000000100110010011001110101010011001101111111101010101110011000"; 
        grayCode = grayCode.replaceAll("\\s+", "");
        int nbits = 1;
        
        //Tamaño de n Bits
        for (int i = 0; i < grayCode.length(); i++) {
            if (i * Math.pow(2, i) == grayCode.length()) {
                nbits = i;
                break;
            }
 
        }
        System.out.println("Cadena de Gray :\n"+grayCode);
        if (esCodigoGray(grayCode, nbits)) {
            System.out.println("La cadena es un código Gray válido.");
        } else {
            System.out.println("La cadena no es un código Gray válido.");
        }
        System.out.println("Numero de Bits : "+nbits);
    }

    public static boolean esCodigoGray(String cadena, int nbits) {
        // Verificar que la nbits de la cadena sea un múltiplo de la nbits del código Gray
        if (cadena.length() % nbits != 0) {
            return false;
        }

        // Verificar que la cadena solo contiene 0s y 1s
        for (char c : cadena.toCharArray()) {
            if (c != '0' && c != '1') {
                return false;
            }
        }

        // Verificar si la cadena cumple con la propiedad del código Gray
        for (int i = 0; i < cadena.length() - nbits; i += nbits) {
            String actual = cadena.substring(i, i + nbits);
            String siguiente = cadena.substring(i + nbits, i + nbits * 2);

            if (cuentaBitsDiferentes(actual, siguiente) != 1) {
                return false;
            }
        }

        return true;
    }

    public static int cuentaBitsDiferentes(String a, String b) {
        int contador = 0;
        for (int i = 0; i < a.length(); i++) {
            if (a.charAt(i) != b.charAt(i)) {
                contador++;
            }
        }
        return contador;
    }
}