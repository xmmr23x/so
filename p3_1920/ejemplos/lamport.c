// Variables globales compartidas
BOOLEAN Eligiendo[N];
int Turno[N];

// Inicialización de variables
Eligiendo[] = {FALSE, FALSE,..., FALSE};
Turno[] = {0, 0,..., 0};

// Proceso o hilo I-ésimo
void Proceso(int I)
{
    extern BOOLEAN Eligiendo[N];
    extern int Turno[N];
    int j;


    while (TRUE) {
        Eligiendo[I] = TRUE;
        Turno[I] = max(Turno[0], Turno[1],..., Turno[N - 1]) + 1;
        Eligiendo[I] = FALSE;
        for (j = 0; j < N; j++) {
            while (Eligiendo[j]);
            while ((Turno[j] != 0) && (Turno[j], j) < (Turno[I], I));
        }
        //--------------------
        // SECCIÓN CRÍTICA
        //--------------------
        Turno[I] = 0;
        // Sección residual
    }
}

