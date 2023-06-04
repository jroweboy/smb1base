object FormPreferences: TFormPreferences
  Left = 195
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Preferences'
  ClientHeight = 300
  ClientWidth = 427
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 427
    Height = 266
    Align = alClient
    BevelOuter = bvNone
    BorderWidth = 5
    ParentColor = True
    TabOrder = 1
    object PageControl1: TPageControl
      Left = 5
      Top = 5
      Width = 417
      Height = 256
      ActivePage = TabSheet4
      Align = alClient
      TabOrder = 0
      object TabSheet1: TTabSheet
        Caption = 'Startup'
        object RGroupScale: TGroupBox
          Left = 3
          Top = 0
          Width = 131
          Height = 41
          Caption = 'GUI Scale'
          TabOrder = 0
          object RadioScale2x: TRadioButton
            Left = 11
            Top = 16
            Width = 33
            Height = 17
            Caption = '2x'
            TabOrder = 0
          end
          object RadioScale3x: TRadioButton
            Left = 44
            Top = 16
            Width = 33
            Height = 17
            Caption = '3x'
            TabOrder = 1
          end
          object RadioScale4x: TRadioButton
            Left = 77
            Top = 16
            Width = 33
            Height = 17
            Caption = '4x'
            TabOrder = 2
          end
        end
        object RGroupColour: TGroupBox
          Left = 140
          Top = 0
          Width = 130
          Height = 41
          Caption = 'Preselected colour'
          TabOrder = 1
          object RadioCol0: TRadioButton
            Left = 9
            Top = 16
            Width = 25
            Height = 17
            Caption = '0'
            TabOrder = 0
          end
          object RadioCol1: TRadioButton
            Left = 39
            Top = 16
            Width = 25
            Height = 17
            Caption = '1'
            TabOrder = 1
          end
          object RadioCol2: TRadioButton
            Left = 70
            Top = 16
            Width = 29
            Height = 17
            Caption = '2'
            TabOrder = 2
          end
          object RadioCol3: TRadioButton
            Left = 99
            Top = 16
            Width = 25
            Height = 17
            Caption = '3'
            TabOrder = 3
          end
        end
        object RGroupSubpal: TGroupBox
          Left = 276
          Top = 0
          Width = 130
          Height = 41
          Caption = 'Preselected subpalette'
          TabOrder = 2
          object RadioSubpal0: TRadioButton
            Left = 9
            Top = 16
            Width = 28
            Height = 17
            Caption = '0'
            TabOrder = 0
          end
          object RadioSubpal1: TRadioButton
            Left = 38
            Top = 16
            Width = 26
            Height = 17
            Caption = '1'
            TabOrder = 1
          end
          object RadioSubpal2: TRadioButton
            Left = 65
            Top = 16
            Width = 29
            Height = 17
            Caption = '2'
            TabOrder = 2
          end
          object RadioSubpal3: TRadioButton
            Left = 92
            Top = 16
            Width = 25
            Height = 17
            Caption = '3'
            TabOrder = 3
          end
        end
        object RGroupGrid: TGroupBox
          Left = 3
          Top = 40
          Width = 131
          Height = 124
          Caption = 'Grid'
          TabOrder = 3
          object RadioGridHide: TRadioButton
            Left = 11
            Top = 16
            Width = 41
            Height = 17
            Caption = 'Off'
            TabOrder = 0
          end
          object RadioGridShow: TRadioButton
            Left = 60
            Top = 16
            Width = 48
            Height = 17
            Caption = 'On'
            TabOrder = 1
          end
          object CheckGrid1: TCheckBox
            Left = 11
            Top = 34
            Width = 81
            Height = 17
            Caption = 'x1 (tile grid)'
            TabOrder = 2
          end
          object CheckGrid2: TCheckBox
            Left = 11
            Top = 50
            Width = 105
            Height = 17
            Caption = 'x2 (attribute grid)'
            TabOrder = 3
          end
          object CheckGrid4: TCheckBox
            Left = 11
            Top = 66
            Width = 97
            Height = 17
            Caption = 'x4 (block grid)'
            TabOrder = 4
          end
          object CheckGridPixelCHR: TCheckBox
            Left = 11
            Top = 102
            Width = 115
            Height = 17
            Caption = 'Pixel grid on (CHR)'
            TabOrder = 5
          end
          object CheckGrid32x30: TCheckBox
            Left = 11
            Top = 82
            Width = 97
            Height = 17
            Caption = '32x30 (screen)'
            TabOrder = 6
          end
        end
        object GroupShowWindow: TGroupBox
          Left = 3
          Top = 165
          Width = 131
          Height = 60
          Caption = 'Show window'
          TabOrder = 4
          object CheckShowCHREditor: TCheckBox
            Left = 11
            Top = 16
            Width = 97
            Height = 17
            Caption = 'CHR editor'
            TabOrder = 0
          end
          object CheckShowMetaspriteManager: TCheckBox
            Left = 11
            Top = 32
            Width = 116
            Height = 17
            Caption = 'Metasprite manager'
            TabOrder = 1
          end
        end
      end
      object TabSheet2: TTabSheet
        Caption = 'Editor'
        object GroupBox3: TGroupBox
          Left = 3
          Top = 3
          Width = 130
          Height = 40
          Caption = 'Recall colour [Z]'
          TabOrder = 8
          object CheckAutostoreLastUsed: TCheckBox
            Left = 9
            Top = 16
            Width = 118
            Height = 17
            Caption = 'autostore last used'
            TabOrder = 0
          end
        end
        object GroupBitmask: TGroupBox
          Left = 137
          Top = 3
          Width = 99
          Height = 111
          Caption = 'Apply bitmask to'
          TabOrder = 2
          object CheckBitmaskPen: TCheckBox
            Left = 11
            Top = 16
            Width = 38
            Height = 17
            Caption = 'Pen'
            TabOrder = 4
          end
          object CheckBitmaskMirror: TCheckBox
            Left = 11
            Top = 32
            Width = 49
            Height = 17
            Caption = 'Mirror'
            TabOrder = 0
          end
          object CheckBitmaskRotate: TCheckBox
            Left = 11
            Top = 48
            Width = 57
            Height = 17
            Caption = 'Rotate'
            TabOrder = 1
          end
          object CheckBitmaskNudge: TCheckBox
            Left = 11
            Top = 64
            Width = 49
            Height = 17
            Caption = 'Nudge'
            TabOrder = 2
          end
          object CheckBitmaskPaste: TCheckBox
            Left = 11
            Top = 80
            Width = 49
            Height = 17
            Caption = 'Paste'
            TabOrder = 3
          end
        end
        object GroupRules: TGroupBox
          Left = 242
          Top = 3
          Width = 164
          Height = 52
          Caption = 'Rules'
          TabOrder = 1
          object CheckRules0F: TCheckBox
            Left = 11
            Top = 16
            Width = 102
            Height = 17
            Caption = 'Use $0F as black'
            TabOrder = 0
          end
          object CheckRulesSharedBG: TCheckBox
            Left = 11
            Top = 32
            Width = 145
            Height = 17
            Caption = 'Shared background colour'
            TabOrder = 1
          end
        end
        object RGroupASCIIBase: TGroupBox
          Left = 242
          Top = 56
          Width = 164
          Height = 40
          Caption = 'Type In ASCII Base Offset'
          TabOrder = 3
          object RadioASCIIneg20: TRadioButton
            Left = 11
            Top = 16
            Width = 45
            Height = 17
            Caption = '-$20'
            TabOrder = 0
          end
          object RadioASCIIneg30: TRadioButton
            Left = 61
            Top = 16
            Width = 45
            Height = 17
            Caption = '-$30'
            TabOrder = 1
          end
          object RadioASCIIneg40: TRadioButton
            Left = 112
            Top = 16
            Width = 45
            Height = 17
            Caption = '-$40'
            TabOrder = 2
          end
        end
        object GroupFindUnused: TGroupBox
          Left = 242
          Top = 98
          Width = 164
          Height = 71
          Caption = 'Find unused'
          TabOrder = 4
          object CheckFindUnusedForce: TCheckBox
            Left = 11
            Top = 16
            Width = 137
            Height = 17
            Caption = 'force: only on active tab'
            TabOrder = 0
          end
          object CheckFindUnusedName: TCheckBox
            Left = 11
            Top = 31
            Width = 142
            Height = 18
            Caption = 'include nametable/map'
            TabOrder = 1
          end
          object CheckFindUnusedMeta: TCheckBox
            Left = 11
            Top = 46
            Width = 113
            Height = 19
            Caption = 'include metasprites'
            TabOrder = 2
          end
        end
        object GroupRemoveFound: TGroupBox
          Left = 242
          Top = 171
          Width = 164
          Height = 54
          Caption = 'Remove found'
          TabOrder = 5
          object CheckRemoveFoundSort: TCheckBox
            Left = 11
            Top = 16
            Width = 129
            Height = 17
            Caption = '+ sort tiles afterwards'
            TabOrder = 0
          end
        end
        object RGroupInkLimit: TGroupBox
          Left = 3
          Top = 120
          Width = 128
          Height = 52
          Caption = 'Inc/dec ink limit'
          TabOrder = 6
          object RadioInkLimitCap: TRadioButton
            Left = 11
            Top = 16
            Width = 41
            Height = 17
            Caption = 'Cap'
            TabOrder = 0
          end
          object RadioInkLimitWrap: TRadioButton
            Left = 11
            Top = 32
            Width = 45
            Height = 17
            Caption = 'Wrap'
            TabOrder = 1
          end
        end
        object RGroupInkBehaviour: TGroupBox
          Left = 3
          Top = 173
          Width = 128
          Height = 52
          Caption = 'Inc/dec ink behaviour'
          TabOrder = 0
          object RadioInkBehaviourClick: TRadioButton
            Left = 11
            Top = 16
            Width = 63
            Height = 17
            Caption = 'Per click'
            TabOrder = 0
          end
          object RadioInkBehaviourDistance: TRadioButton
            Left = 11
            Top = 32
            Width = 97
            Height = 17
            Caption = 'Over distance'
            TabOrder = 1
          end
        end
        object RGroupInkFlow: TGroupBox
          Left = 137
          Top = 120
          Width = 99
          Height = 105
          Caption = 'Inc/dec ink flow'
          TabOrder = 7
          object RadioInkFlowQuickest: TRadioButton
            Left = 11
            Top = 16
            Width = 65
            Height = 17
            Caption = 'Quickest'
            TabOrder = 0
          end
          object RadioInkFlowQuick: TRadioButton
            Left = 11
            Top = 32
            Width = 49
            Height = 17
            Caption = 'Quick'
            TabOrder = 1
          end
          object RadioInkFlowMedium: TRadioButton
            Left = 11
            Top = 48
            Width = 57
            Height = 17
            Caption = 'Medium'
            TabOrder = 2
          end
          object RadioInkFlowSlow: TRadioButton
            Left = 11
            Top = 64
            Width = 49
            Height = 17
            Caption = 'Slow'
            TabOrder = 3
          end
          object RadioInkFlowSlowest: TRadioButton
            Left = 11
            Top = 80
            Width = 62
            Height = 17
            Caption = 'Slowest'
            TabOrder = 4
          end
        end
      end
      object TabSheet3: TTabSheet
        Caption = 'Import'
        object GroupBMPAsName: TGroupBox
          Left = 3
          Top = 3
          Width = 137
          Height = 89
          Caption = 'BMP as nametable'
          TabOrder = 0
          object CheckBMPBestOffsets: TCheckBox
            Left = 11
            Top = 16
            Width = 97
            Height = 17
            Caption = 'Best offsets'
            TabOrder = 0
          end
          object CheckBMPLossy: TCheckBox
            Left = 11
            Top = 32
            Width = 112
            Height = 17
            Caption = 'Lossy import (slow)'
            TabOrder = 1
          end
          object CheckBMPThres: TCheckBox
            Left = 11
            Top = 48
            Width = 97
            Height = 17
            Caption = 'Threshold'
            TabOrder = 3
          end
          object CheckBMPNoColour: TCheckBox
            Left = 11
            Top = 64
            Width = 117
            Height = 17
            Caption = 'Without colour data'
            TabOrder = 2
          end
        end
      end
      object TabSheet4: TTabSheet
        Caption = 'Export'
        object RGroupSpriteMetadata: TGroupBox
          Left = 253
          Top = 3
          Width = 153
          Height = 121
          Caption = 'Sprite export metadata'
          TabOrder = 0
          object RadioNoHeader: TRadioButton
            Left = 11
            Top = 17
            Width = 136
            Height = 16
            Caption = 'No header or terminator'
            TabOrder = 0
          end
          object RadioSpriteCount: TRadioButton
            Left = 11
            Top = 32
            Width = 121
            Height = 17
            Caption = 'Sprite count header'
            TabOrder = 1
          end
          object RadioNflag: TRadioButton
            Left = 11
            Top = 48
            Width = 129
            Height = 17
            Caption = '$80 (N flag) terminator'
            TabOrder = 2
          end
          object RadioFFTerminator: TRadioButton
            Left = 11
            Top = 64
            Width = 113
            Height = 17
            Caption = '$FF terminator'
            TabOrder = 3
          end
          object RadioSingle00: TRadioButton
            Left = 11
            Top = 80
            Width = 113
            Height = 17
            Caption = '$00 terminator'
            TabOrder = 5
          end
          object RadioDouble00: TRadioButton
            Left = 11
            Top = 96
            Width = 129
            Height = 17
            Caption = 'Double $00 terminator'
            TabOrder = 4
          end
        end
        object GroupAskName: TGroupBox
          Left = 253
          Top = 126
          Width = 153
          Height = 57
          Caption = 'Metasprite/bank to clipboard'
          TabOrder = 1
          object CheckAskSprName: TCheckBox
            Left = 11
            Top = 16
            Width = 121
            Height = 17
            Caption = 'Ask metasprite name'
            TabOrder = 0
          end
          object CheckAskBankName: TCheckBox
            Left = 11
            Top = 32
            Width = 97
            Height = 17
            Caption = 'Ask bank name'
            TabOrder = 1
          end
        end
        object GroupAsmSyntax: TGroupBox
          Left = 253
          Top = 185
          Width = 153
          Height = 39
          Caption = 'Asm syntax'
          TabOrder = 2
          object RadioAsmByte: TRadioButton
            Left = 11
            Top = 16
            Width = 48
            Height = 17
            Caption = '.byte'
            TabOrder = 0
          end
          object RadioAsmDb: TRadioButton
            Left = 80
            Top = 16
            Width = 34
            Height = 17
            Caption = '.db'
            TabOrder = 1
          end
        end
        object GroupBox1: TGroupBox
          Left = 3
          Top = 3
          Width = 129
          Height = 81
          Caption = 'Save map/screen'
          TabOrder = 3
          object CheckIncludeNames: TCheckBox
            Left = 11
            Top = 16
            Width = 105
            Height = 17
            Caption = 'Include names'
            TabOrder = 0
          end
          object CheckIncludeAttributes: TCheckBox
            Left = 11
            Top = 32
            Width = 105
            Height = 17
            Caption = 'Include attributes'
            TabOrder = 1
          end
          object CheckRLECompress: TCheckBox
            Left = 11
            Top = 48
            Width = 105
            Height = 17
            Caption = 'Force NESlib RLE'
            TabOrder = 2
          end
        end
        object GroupBox11: TGroupBox
          Left = 135
          Top = 3
          Width = 112
          Height = 81
          Caption = 'Palette to clipboard'
          TabOrder = 4
          object CheckExportPalFilename: TCheckBox
            Left = 11
            Top = 16
            Width = 97
            Height = 17
            Caption = 'Filename in label'
            TabOrder = 0
          end
          object CheckExportPalSet: TCheckBox
            Left = 11
            Top = 32
            Width = 97
            Height = 17
            Caption = 'Set # in label'
            TabOrder = 1
          end
        end
      end
      object TabSheet5: TTabSheet
        Caption = 'Grids and guides'
        ImageIndex = 4
        object GroupBox4: TGroupBox
          Left = 3
          Top = 3
          Width = 126
          Height = 39
          Caption = 'Autoshow grid when'
          TabOrder = 0
          object CheckAutoshowDrag: TCheckBox
            Left = 11
            Top = 16
            Width = 112
            Height = 17
            Caption = 'dragging content'
            TabOrder = 0
          end
        end
        object GroupBox5: TGroupBox
          Left = 144
          Top = 58
          Width = 262
          Height = 72
          Caption = 'Metasprite warnings'
          TabOrder = 1
          object CheckMsprYellow: TCheckBox
            Left = 11
            Top = 16
            Width = 248
            Height = 17
            Caption = 'Yellow: Just 1 of this metasprite fits at scanline'
            TabOrder = 0
          end
          object CheckMsprOrange: TCheckBox
            Left = 11
            Top = 32
            Width = 237
            Height = 17
            Caption = 'Orange: Metasprite is at scanline limit (8)'
            TabOrder = 1
          end
          object CheckMsprRed: TCheckBox
            Left = 11
            Top = 48
            Width = 230
            Height = 17
            Caption = 'Red: This metasprite alone causes flicker'
            TabOrder = 2
          end
        end
        object GroupBox6: TGroupBox
          Left = 144
          Top = 130
          Width = 128
          Height = 95
          Caption = '32x30 grid: Navigator'
          TabOrder = 2
          object RadioNavScrAlways: TRadioButton
            Left = 11
            Top = 14
            Width = 66
            Height = 17
            Caption = 'Always'
            TabOrder = 0
          end
          object RadioNavScrMouse: TRadioButton
            Left = 11
            Top = 29
            Width = 77
            Height = 17
            Caption = 'Mouse over'
            TabOrder = 1
          end
          object RadioNavScrMB: TRadioButton
            Left = 11
            Top = 44
            Width = 113
            Height = 17
            Caption = 'M. over / Button'
            TabOrder = 2
          end
          object RadioNavScrButton: TRadioButton
            Left = 11
            Top = 59
            Width = 113
            Height = 17
            Caption = 'Button down'
            TabOrder = 3
          end
          object RadioNavScrNever: TRadioButton
            Left = 11
            Top = 74
            Width = 113
            Height = 17
            Caption = 'Never'
            TabOrder = 4
          end
        end
        object GroupBox7: TGroupBox
          Left = 278
          Top = 130
          Width = 128
          Height = 95
          Caption = '32x30 grid: main canvas'
          TabOrder = 3
          object RadioMainScrAlways: TRadioButton
            Left = 11
            Top = 14
            Width = 113
            Height = 17
            Caption = 'Always'
            TabOrder = 0
          end
          object RadioMainScrMouse: TRadioButton
            Left = 11
            Top = 29
            Width = 113
            Height = 17
            Caption = 'Mouse over'
            TabOrder = 1
          end
          object RadioMainScrMB: TRadioButton
            Left = 11
            Top = 44
            Width = 113
            Height = 17
            Caption = 'M. over / Button'
            TabOrder = 2
          end
          object RadioMainScrButton: TRadioButton
            Left = 11
            Top = 59
            Width = 113
            Height = 17
            Caption = 'Button down'
            TabOrder = 3
          end
          object RadioMainScrNever: TRadioButton
            Left = 11
            Top = 74
            Width = 113
            Height = 17
            Caption = 'Never'
            TabOrder = 4
          end
        end
      end
      object TabSheet6: TTabSheet
        Caption = 'Workspace'
        ImageIndex = 5
        object RGroupWorkspace: TGroupBox
          Left = 151
          Top = 3
          Width = 128
          Height = 52
          Caption = 'Workspace: Metasprites'
          TabOrder = 0
          object RadioSprlistLeft: TRadioButton
            Left = 11
            Top = 16
            Width = 82
            Height = 17
            Caption = 'Spritelist: left'
            TabOrder = 1
          end
          object RadioSprlistCenter: TRadioButton
            Left = 11
            Top = 31
            Width = 98
            Height = 17
            Caption = 'Spritelist: center'
            TabOrder = 0
          end
        end
        object GroupBox2: TGroupBox
          Left = 278
          Top = 3
          Width = 128
          Height = 52
          Caption = 'Workspace: CHR'
          TabOrder = 1
          object RadioToolTop: TRadioButton
            Left = 11
            Top = 16
            Width = 113
            Height = 17
            Caption = 'Toolbar: top'
            TabOrder = 0
          end
          object RadioToolBottom: TRadioButton
            Left = 11
            Top = 32
            Width = 113
            Height = 17
            Caption = 'Toolbar: bottom'
            TabOrder = 1
          end
        end
        object GroupBox8: TGroupBox
          Left = 3
          Top = 3
          Width = 142
          Height = 37
          Caption = 'Snap'
          TabOrder = 2
          object CheckFormToMonitor: TCheckBox
            Left = 11
            Top = 16
            Width = 128
            Height = 17
            Caption = 'Forms to Monitor edge'
            TabOrder = 0
          end
        end
        object GroupBox9: TGroupBox
          Left = 151
          Top = 57
          Width = 255
          Height = 52
          Caption = 'Lightbox mode: alpha value'
          TabOrder = 3
          object TrackBarAlpha: TTrackBar
            Left = 3
            Top = 17
            Width = 249
            Height = 27
            Max = 220
            Min = 140
            Frequency = 8
            Position = 180
            TabOrder = 0
            ThumbLength = 16
          end
        end
        object GroupBox10: TGroupBox
          Left = 3
          Top = 42
          Width = 142
          Height = 67
          Caption = 'Menu: open/save binaries'
          TabOrder = 4
          object RadioOpenSave1: TRadioButton
            Left = 11
            Top = 16
            Width = 113
            Height = 17
            Caption = 'in File menu (new)'
            TabOrder = 0
          end
          object RadioOpenSave2: TRadioButton
            Left = 11
            Top = 31
            Width = 128
            Height = 17
            Caption = 'in type menus (classic)'
            TabOrder = 1
          end
          object RadioOpenSave3: TRadioButton
            Left = 11
            Top = 46
            Width = 113
            Height = 17
            Caption = 'in both'
            TabOrder = 2
          end
        end
      end
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 266
    Width = 427
    Height = 34
    Align = alBottom
    BevelOuter = bvNone
    ParentColor = True
    TabOrder = 0
    object OKBtn: TButton
      Left = 248
      Top = 2
      Width = 94
      Height = 25
      Caption = 'Apply and save'
      Default = True
      ModalResult = 1
      TabOrder = 0
      OnClick = OKBtnClick
    end
    object CancelBtn: TButton
      Left = 347
      Top = 2
      Width = 75
      Height = 25
      Cancel = True
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 2
    end
    object HelpBtn: TButton
      Left = 5
      Top = 2
      Width = 123
      Height = 25
      Caption = '&Reload install settings'
      TabOrder = 1
      OnClick = HelpBtnClick
    end
  end
end
