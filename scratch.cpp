int *rowVal = new int[diagElements+nonDiagElements]{0};
    int *colVal = new int[diagElements+nonDiagElements]{0};
    double *values = new double[diagElements+nonDiagElements]{0};
    
    uniform_int_distribution<> diagIndex(low,high-1);
    // default_random_engine diagIndices;
    for (int i = low; i<high;i++){
        // memset(values, 0, sizeof(values));
        if (low==0){
            uniform_int_distribution<> nonDiagIndex1(high,m-1);
            uniform_int_distribution<> nonDiagIndex2(high,m-1);
            uniform_int_distribution<> selector(0,1);
            //for diagonal elements
            for(int j = 0;j<diagElements;j++){
                rowVal[j]=i;
                colVal[j] =diagIndex(eng);
                values[j]  = randomDouble(eng);
            }
            //for non diagonal elements
            for(int j = diagElements; j<(nonDiagElements+nonDiagElements);j++){
                indexSelect = selector(eng);
                values[j] = randomDouble(eng);
                _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
                rowVal[j] = i;
                colVal[j] = _tempIndex;
            }
        } else if (high==m){
            uniform_int_distribution<> nonDiagIndex1(0,low-1);
            uniform_int_distribution<> nonDiagIndex2(0,low-1);
            uniform_int_distribution<> selector(0,1);
            //for diagonal elements
            for(int j = 0;j<diagElements;j++){
                rowVal[j]=i;
                colVal[j] =diagIndex(eng);
                values[j]  = randomDouble(eng);
            }
            //for non diagonal elements
            for(int j = 0; j<(nonDiagElements+nonDiagElements);j++){
                indexSelect = selector(eng);
                values[j] = randomDouble(eng);
                _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
                rowVal[j] = i;
                colVal[j] = _tempIndex;
            }
    
        } else{
            uniform_int_distribution<> nonDiagIndex1(0,low-1);
            uniform_int_distribution<> nonDiagIndex2(high,m-1);
            uniform_int_distribution<> selector(0,1);
            //for diagonal elements
            for(int j = 0;j<diagElements;j++){
                rowVal[j]=i;
                colVal[j] =diagIndex(eng);
                values[j]  = randomDouble(eng);
            }
            //for non diagonal elements
            for(int j = 0; j<(nonDiagElements+nonDiagElements);j++){
                indexSelect = selector(eng);
                values[j] = randomDouble(eng);
                _tempIndex = nonDiagIndex1(eng)*indexSelect + abs(indexSelect-1)*nonDiagIndex2(eng);
                rowVal[j] = i;
                colVal[j] = _tempIndex;
            }
        }
        MatSetValues(A,1,)
    }
    