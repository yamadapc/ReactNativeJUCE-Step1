import React, {Component} from 'react';
import {ComponentRegistry, View, Text} from 'react-native-juce';

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

ComponentRegistry.register('HelloWorld', HelloWorld);
