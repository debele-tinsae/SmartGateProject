package com.example.Smart_Gate;


import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

public class SignInActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.signin_actvity);

            TextView textview = findViewById(R.id.welcome);

            String welcome = "welcome to Smart Gate";


            textview.setText(welcome);

            TextView trail = findViewById(R.id.editText);

          String shady =   trail.getText().toString();


    }
}