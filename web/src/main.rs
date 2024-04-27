use std::{
    fs,
    io::{prelude::*, BufReader},
    net::{TcpListener, TcpStream},
};

//the idea is to have the esp32 act as a server where some information is held
//the esp32 will be hosting an http server, we can send a GET request to it from here and receive
//the temperature and humidity
//then, we expect a json response, and we can update the web page 

fn main() {
    let listener = TcpListener::bind("10.0.0.180:8000").unwrap();

    for connection in listener.incoming()
    {
        let connection = connection.unwrap();

        handle_connection(connection);

        println!("New connection");
    }
}

fn handle_connection(mut connection: TcpStream)
{
    let buf_reader = BufReader::new(&mut connection);
    let _http_request: Vec<_> = buf_reader
        .lines()
        .map(|result| result.unwrap())
        .take_while(|line| !line.is_empty())
        .collect();

    let status_response =  "HTTP/1.1 200 OK";
    let contents = fs::read_to_string("index.html").unwrap();
    let length = contents.len();

    let response = format!("{status_response}\r\nContent-Length: {length}\r\n\r\n{contents}");
    connection.write_all(response.as_bytes()).unwrap();    
}
