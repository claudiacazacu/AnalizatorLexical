fn main() {
    let num1 = 10;
    let num2 = 5;

    // Addition operation
    let addition_result = num1 + num2;
    println!("Addition result: {}", addition_result);

    // Division operation
    // Check if the divisor is not zero before performing division
    if num2 != 0 {
        let division_result = num1 / num2;
        println!("Division result: {}", division_result);
    } else {
        println!("Cannot divide by zero!");
    }
}
