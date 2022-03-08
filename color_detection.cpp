if(analogRead(IR1)< threshold && analogRead(IR2)>=threshold && analogRead(IR3)<threshold) //IR will not glow on black line
  { 
   Serial.println("");
  }

  else if(analogRead(IR1)>= threshold && analogRead(IR2)<threshold && analogRead(IR3)<threshold)  //IR not on black line
  {
    
   Serial.println("");
  }

  else if(analogRead(IR1)< threshold && analogRead(IR2)<threshold && analogRead(IR3)>=threshold)
  {
    Serial.println("");
  }