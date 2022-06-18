package com.example.Smart_Gate;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.TextView;
import android.view.View;


public class RegisterActivity extends AppCompatActivity {


    private users_info Registrar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_register);

        Intent intent = getIntent();





    }

    private void setUsers(){


        TextView user = findViewById(R.id.Username);
        TextView pass = findViewById(R.id.Password);


        String username = user.getText().toString();
        String password = pass.getText().toString();

        Registrar.users_info(username,password);


    }


    public String getUsername(){

        TextView user = findViewById(R.id.Username);

        return user.getText().toString();

    }


    public String getPassword(){


        TextView pass = findViewById(R.id.Password);

        return pass.getText().toString();

    }


    public void Add_user(View view) {

            TextView apartmentNumber = findViewById(R.id.ApartmentNum);
            TextView textview = findViewById(R.id.statusofadddeduser);

            int aprtNum = Integer.parseInt(apartmentNumber.getText().toString());


            if(aprtNum < 10)

            {
                setUsers();


                textview.setText("successfully added");

            }

            else{

                textview.setText("Unsuccessful");
            }


    }
}
