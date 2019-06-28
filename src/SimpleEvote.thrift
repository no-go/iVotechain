struct Ident {
    1:i32       id,
    2:string  name,
    3:bool    used = false
}

service RegOffice {
    string              sign( 1:Ident ident, 2:string blindedHashedPsydonyme )
}

service BallotBox {
    bool                vote( 1:string     pseudonym, 2:string signature, 3:string pubCryptVote ),
    list<string> endElection( 1:string privBase64Key )
}
