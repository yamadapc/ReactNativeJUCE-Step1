import React, {Component} from 'react';
import {render, ComponentRegistry, View, Text} from 'react-native-juce';

class HelloWorld extends Component {
  render() {
    return (
      <View>
        <Text>
          Hello World
        </Text>
      </View>
    );
  }
}

render(<HelloWorld />);
