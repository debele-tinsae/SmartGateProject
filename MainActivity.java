package com.example.Smart_Gate;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {


    @Override
    
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


    }

        public void Register(View view){

        //creating the registration page by onclick to registration button
            Intent intent = new Intent(this, RegisterActivity.class);

            startActivity(intent);



        }


        public void sign_In(View view){

        //creating the sign-in activity


        //textview username and password

        final EditText user = (EditText) findViewById(R.id.username);

        final EditText password = (EditText) findViewById(R.id.password);

        String user_test = "Admin";

        String pass_test = "admin";


    //Check whether is the sign-in button works

        if(user.getText().toString().equals(user_test) && password.getText().toString().equals(pass_test)){

            //start a new intent page if signin is correct to the test values

            Intent intent = new Intent(this, SignInActivity.class);

            startActivity(intent);


        }


        else{

            //incorrect username or password shown to incorrect sign-in username and password

            TextView incorrect =(TextView) findViewById(R.id.status);

            String Incorrect = "Incorrect Username or Password";

            incorrect.setText(Incorrect);

        }



        }
    }

